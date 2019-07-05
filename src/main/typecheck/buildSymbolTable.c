// Copyright 2019 Justin Hu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file is part of the T Language Compiler.

// Implementation of type checking

#include "typecheck/buildSymbolTable.h"

// helpers
static Type *astToType(Node const *ast, Report *report, Options const *options,
                       Environment const *env, char const *filename) {
  switch (ast->type) {
    case NT_KEYWORDTYPE: {
      return keywordTypeCreate(ast->data.typeKeyword.type - TK_VOID + K_VOID);
    }
    case NT_ID: {
      SymbolInfo *info = environmentLookup(env, report, ast->data.id.id,
                                           ast->line, ast->character, filename);
      return info == NULL
                 ? NULL
                 : referneceTypeCreate(
                       info->data.type.kind - TDK_STRUCT + K_STRUCT, info);
    }
    case NT_CONSTTYPE: {
      if (ast->data.constType.target->type == NT_CONSTTYPE) {
        switch (optionsGet(options, optionWDuplicateDeclSpecifier)) {
          case O_WT_ERROR: {
            reportError(report,
                        "%s:%zu:%zu: error: duplciate 'const' specifier",
                        filename, ast->line, ast->character);
            return NULL;
          }
          case O_WT_WARN: {
            reportWarning(report,
                          "%s:%zu:%zu: warning: duplciate 'const' specifier",
                          filename, ast->line, ast->character);
            return astToType(ast->data.constType.target, report, options, env,
                             filename);
          }
          case O_WT_IGNORE: {
            return astToType(ast->data.constType.target, report, options, env,
                             filename);
          }
        }
      }

      Type *subType =
          astToType(ast->data.constType.target, report, options, env, filename);
      return subType == NULL ? NULL : modifierTypeCreate(K_PTR, subType);
    }
    case NT_ARRAYTYPE: {
      Node const *sizeConst = ast->data.arrayType.size;
      switch (sizeConst->data.constExp.type) {
        case CT_UBYTE:
        case CT_USHORT:
        case CT_UINT:
        case CT_ULONG: {
          break;
        }
        default: {
          reportError(report,
                      "%s:%zu:%zu: error: expected an unsigned integer for an "
                      "array size, but found %s",
                      filename, sizeConst->line, sizeConst->character,
                      constTypeToString(sizeConst->data.constExp.type));
          return NULL;
        }
      }
      size_t size;
      switch (sizeConst->data.constExp.type) {
        case CT_UBYTE: {
          size = sizeConst->data.constExp.value.ubyteVal;
          break;
        }
        case CT_USHORT: {
          size = sizeConst->data.constExp.value.ushortVal;
          break;
        }
        case CT_UINT: {
          size = sizeConst->data.constExp.value.uintVal;
          break;
        }
        case CT_ULONG: {
          size = sizeConst->data.constExp.value.ulongVal;
          break;
        }
        default: {
          return NULL;  // error: mutation in const ptr
        }
      }
      Type *subType = astToType(ast->data.arrayType.element, report, options,
                                env, filename);
      return subType == NULL ? NULL : arrayTypeCreate(subType, size);
    }
    case NT_PTRTYPE: {
      Type *subType =
          astToType(ast->data.ptrType.target, report, options, env, filename);
      return subType == NULL ? NULL : modifierTypeCreate(K_PTR, subType);
    }
    case NT_FNPTRTYPE: {
      Type *retType = astToType(ast->data.fnPtrType.returnType, report, options,
                                env, filename);
      if (retType == NULL) {
        return NULL;
      }

      TypeVector *argTypes = typeVectorCreate();
      for (size_t idx = 0; idx < ast->data.fnPtrType.argTypes->size; idx++) {
        Type *argType = astToType(ast->data.fnPtrType.argTypes->elements[idx],
                                  report, options, env, filename);
        if (argType == NULL) {
          typeDestroy(retType);
        }

        typeVectorInsert(argTypes, argType);
      }

      return functionPtrTypeCreate(retType, argTypes);
    }
    default: {
      return NULL;  // error: not syntactically valid
    }
  }
}

// expression

// statement

// top level
static void buildStabFunDefn(Node *fn, Report *report, Options const *options,
                             Environment *env, char const *filename) {
  // env has no scopes
  Node *name = fn->data.function.id;
  SymbolInfo *info = symbolTableGet(env->currentModule, name->data.id.id);
  if (info != NULL && info->kind != SK_FUNCTION) {
    // already declared/defined as a non-function - error!
    reportError(report, "%s:%zu:%zu: error: '%s' already defined as %s",
                filename, name->line, name->character, name->data.id.id,
                symbolInfoToKindString(info));
    return;
  } else if (info == NULL) {
    // not declared/defined - do that now
    info = functionSymbolInfoCreate();
    OverloadSetElement *overload = overloadSetElementCreate();

    Type *returnType =
        astToType(fn->data.function.returnType, report, options, env, filename);
    if (returnType == NULL) {
      overloadSetElementDestroy(overload);
      symbolInfoDestroy(info);
      return;
    }
    overload->returnType = returnType;

    for (size_t idx = 0; idx < fn->data.function.formals->size; idx++) {
      Type *argType = astToType(fn->data.function.formals->firstElements[idx],
                                report, options, env, filename);
      if (argType == NULL) {
        overloadSetElementDestroy(overload);
        symbolInfoDestroy(info);
        return;
      }
      typeVectorInsert(&overload->argumentTypes, argType);
    }

    overload->defined = true;
    // overload->numOptional

    overloadSetInsert(&info->data.function.overloadSet, overload);
    symbolTablePut(env->currentModule, name->data.id.id, info);
  } else {
    // is already declared/defined.
    OverloadSetElement *overload = overloadSetElementCreate();
    Type *returnType =
        astToType(fn->data.function.returnType, report, options, env, filename);
    if (returnType == NULL) {
      overloadSetElementDestroy(overload);
      return;
    }
    overload->returnType = returnType;

    TypeVector *args = typeVectorCreate();
    for (size_t idx = 0; idx < fn->data.function.formals->size; idx++) {
      Type *argType = astToType(fn->data.function.formals->firstElements[idx],
                                report, options, env, filename);
      if (argType == NULL) {
        typeVectorDestroy(args);
        overloadSetElementDestroy(overload);
        return;
      }
      typeVectorInsert(args, argType);
    }

    // Type *matchedReturnType = functionSymbolInfoGetMatch(info, args);

    // for (size_t candidateIdx = 0;
    //      candidateIdx < info->data.function.argumentTypeSets.size;
    //      candidateIdx++) {
    //   TypeVector *candidateArgs =
    //       info->data.function.argumentTypeSets.elements[candidateIdx];
    //   if (candidateArgs->size == args->size) {
    //     // possible match b/w candidate args and function args
    //   }
    // }
  }

  name->data.id.symbol = info;
  // must not be declared/defined as a non-function, must not allow a function
  // with the same input args and name to be declared/defined
  // TODO: write this
}
static void buildStabFunDecl(Node *fnDecl, Report *report,
                             Options const *options, Environment *env,
                             char const *filename) {
  // must not be declared as a non-function, must check if a function with the
  // same input args and name is declared/defined
  // TODO: write this
}
static void buildStabVarDecl(Node *varDecl, Report *report,
                             Options const *options, Environment *env,
                             char const *filename) {
  // mu
  // must not allow a var with the same name to be defined/declared twice
  // TODO: write this
}
static void buildStabStructDecl(Node *structDecl, Report *report,
                                Options const *options, Environment *env,
                                char const *filename) {
  // must not allow anything that isn't a struct with the same name to be
  // declared/defined, must not allow a struct with the same name to be defined
  // TODO: write this
}
static void buildStabStructForwardDecl(Node *structForwardDecl, Report *report,
                                       Options const *options, Environment *env,
                                       char const *filename) {
  // must not allow anything that isn't a struct with the same name to be
  // declared/defined, must check if a struct with the same name is
  // declared/defined
  // TODO: write this
}
static void buildStabUnionDecl(Node *unionDecl, Report *report,
                               Options const *options, Environment *env,
                               char const *filename) {
  // must not allow anything that isn't a struct with the same name to be
  // declared/defined, must not allow a union with the same name to be defined
  // TODO: write this
}
static void buildStabUnionForwardDecl(Node *unionForwardDecl, Report *report,
                                      Options const *options, Environment *env,
                                      char const *filename) {
  // must not allow anything that isn't a struct with the same name to be
  // declared/defined, must check if a struct with the same name is
  // declared/defined
  // TODO: write this
}
static void buildStabEnumDecl(Node *enumDecl, Report *report,
                              Options const *options, Environment *env,
                              char const *filename) {
  // must not allow anything that isn't an enum with the same name to be
  // declared/defined, must not allow an enum with the same name to be defined
  // TODO: write this
}
static void buildStabEnumForwardDecl(Node *enumForwardDecl, Report *report,
                                     Options const *options, Environment *env,
                                     char const *filename) {
  // must not allow anything that isn't an enum with the same name to be
  // declared/defined, must check if an enum with the same name is
  // declared/defined
  // TODO: write this
}
static void buildStabTypeDefDecl(Node *typedefDecl, Report *report,
                                 Options const *options, Environment *env,
                                 char const *filename) {
  // TODO: write this
}
static void buildStabBody(Node *body, Report *report, Options const *options,
                          Environment *env, char const *filename, bool isDecl) {
  switch (body->type) {
    case NT_FUNCTION: {
      buildStabFunDefn(body, report, options, env, filename);
      return;
    }
    case NT_FUNDECL: {
      buildStabFunDecl(body, report, options, env, filename);
      return;
    }
    case NT_VARDECL: {
      buildStabVarDecl(body, report, options, env, filename);
      return;
    }
    case NT_STRUCTDECL: {
      buildStabStructDecl(body, report, options, env, filename);
      return;
    }
    case NT_STRUCTFORWARDDECL: {
      buildStabStructForwardDecl(body, report, options, env, filename);
      return;
    }
    case NT_UNIONDECL: {
      buildStabUnionDecl(body, report, options, env, filename);
      return;
    }
    case NT_UNIONFORWARDDECL: {
      buildStabUnionForwardDecl(body, report, options, env, filename);
      return;
    }
    case NT_ENUMDECL: {
      buildStabEnumDecl(body, report, options, env, filename);
      return;
    }
    case NT_ENUMFORWARDDECL: {
      buildStabEnumForwardDecl(body, report, options, env, filename);
      return;
    }
    case NT_TYPEDEFDECL: {
      buildStabTypeDefDecl(body, report, options, env, filename);
      return;
    }
    default: {
      return;  // not syntactically valid - caught at parse time
    }
  }
}

// file level stuff
static void buildStabDecl(Node *ast, Report *report, Options const *options,
                          ModuleAstMap const *decls) {
  ast->data.file.symbols = symbolTableCreate();
  Environment env;
  environmentInit(&env, ast->data.file.symbols,
                  ast->data.file.module->data.module.id->data.id.id);

  // add all imports to env
  for (size_t idx = 0; idx < ast->data.file.imports->size; idx++) {
    Node *import = ast->data.file.imports->elements[idx];
    char const *importedId = import->data.import.id->data.id.id;

    Node *importedAst = moduleAstMapGet(decls, importedId);
    SymbolTable *importedTable = importedAst->data.file.symbols;
    if (importedTable == NULL) {
      buildStabDecl(importedAst, report, options, decls);
      importedTable = importedAst->data.file.symbols;
    }
    moduleTableMapPut(&env.imports, importedId, importedTable);
  }

  // traverse body
  for (size_t idx = 0; idx < ast->data.file.bodies->size; idx++) {
    Node *body = ast->data.file.bodies->elements[idx];
    buildStabBody(body, report, options, &env, ast->data.file.filename, true);
  }

  environmentUninit(&env);
}
static void buildStabCode(Node *ast, Report *report, Options const *options,
                          ModuleAstMap const *decls) {
  ast->data.file.symbols = symbolTableCreate();
  Environment env;
  environmentInit(&env, ast->data.file.symbols,
                  ast->data.file.module->data.module.id->data.id.id);

  // add all imports to env
  for (size_t idx = 0; idx < ast->data.file.imports->size; idx++) {
    Node *import = ast->data.file.imports->elements[idx];
    char const *importedId = import->data.import.id->data.id.id;

    // import must have been typechecked to get here
    moduleTableMapPut(&env.imports, importedId,
                      moduleAstMapGet(decls, importedId)->data.file.symbols);
  }

  // traverse body
  for (size_t idx = 0; idx < ast->data.file.bodies->size; idx++) {
    Node *body = ast->data.file.bodies->elements[idx];
    buildStabBody(body, report, options, &env, ast->data.file.filename, false);
  }

  environmentUninit(&env);
}

void buildSymbolTables(Report *report, Options const *options,
                       ModuleAstMapPair const *asts) {
  for (size_t idx = 0; idx < asts->decls.size; idx++) {
    // if this is a decl, and it hasn't been processed
    if (asts->decls.keys[idx] != NULL &&
        ((Node *)asts->decls.values[idx])->data.file.symbols == NULL) {
      buildStabDecl(asts->decls.values[idx], report, options, &asts->decls);
    }
  }
  for (size_t idx = 0; idx < asts->codes.size; idx++) {
    // all codes that are valid haven't been processed by this loop.
    if (asts->codes.keys[idx] != NULL) {
      buildStabCode(asts->decls.values[idx], report, options, &asts->decls);
    }
  }
}