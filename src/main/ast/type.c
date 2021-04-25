// Copyright 2019-2021 Justin Hu
//
// This file is part of the T Language Compiler.
//
// The T Language Compiler is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// The T Language Compiler is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// the T Language Compiler. If not see <https://www.gnu.org/licenses/>.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// type implementation

#include "ast/type.h"

#include "ast/symbolTable.h"

static Type *typeCreate(TypeKind kind) {
  Type *t = malloc(sizeof(Type));
  t->kind = kind;
  return t;
}
Type *keywordTypeCreate(TypeKeyword keyword) {
  Type *t = typeCreate(TK_KEYWORD);
  t->data.keyword.keyword = keyword;
  return t;
}
Type *qualifiedTypeCreate(Type *base, bool constQual, bool volatileQual) {
  Type *t = typeCreate(TK_QUALIFIED);
  t->data.qualified.constQual = constQual;
  t->data.qualified.volatileQual = volatileQual;
  t->data.qualified.base = base;
  return t;
}
Type *pointerTypeCreate(Type *base) {
  Type *t = typeCreate(TK_POINTER);
  t->data.pointer.base = base;
  return t;
}
Type *arrayTypeCreate(uint64_t length, Type *type) {
  Type *t = typeCreate(TK_ARRAY);
  t->data.array.length = length;
  t->data.array.type = type;
  return t;
}
Type *funPtrTypeCreate(Type *returnType) {
  Type *t = typeCreate(TK_FUNPTR);
  t->data.funPtr.returnType = returnType;
  vectorInit(&t->data.funPtr.argTypes);
  return t;
}
Type *aggregateTypeCreate(void) {
  Type *t = typeCreate(TK_AGGREGATE);
  vectorInit(&t->data.aggregate.types);
  return t;
}
Type *referenceTypeCreate(SymbolTableEntry *entry, char *id) {
  Type *t = typeCreate(TK_REFERENCE);
  t->data.reference.entry = entry;
  t->data.reference.id = id;
  return t;
}
Type *typeCopy(Type const *t) {
  switch (t->kind) {
    case TK_KEYWORD: {
      return keywordTypeCreate(t->data.keyword.keyword);
    }
    case TK_QUALIFIED: {
      return qualifiedTypeCreate(typeCopy(t->data.qualified.base),
                                 t->data.qualified.constQual,
                                 t->data.qualified.volatileQual);
    }
    case TK_POINTER: {
      return pointerTypeCreate(typeCopy(t->data.pointer.base));
    }
    case TK_ARRAY: {
      return arrayTypeCreate(t->data.array.length,
                             typeCopy(t->data.array.type));
    }
    case TK_FUNPTR: {
      Type *copy = funPtrTypeCreate(typeCopy(t->data.funPtr.returnType));
      for (size_t idx = 0; idx < t->data.funPtr.argTypes.size; ++idx)
        vectorInsert(&copy->data.funPtr.argTypes,
                     typeCopy(t->data.funPtr.argTypes.elements[idx]));
      return copy;
    }
    case TK_AGGREGATE: {
      Type *copy = aggregateTypeCreate();
      for (size_t idx = 0; idx < t->data.aggregate.types.size; ++idx)
        vectorInsert(&copy->data.funPtr.argTypes,
                     typeCopy(t->data.aggregate.types.elements[idx]));
      return copy;
    }
    case TK_REFERENCE: {
      return referenceTypeCreate(t->data.reference.entry,
                                 strdup(t->data.reference.id));
    }
    default: {
      error(__FILE__, __LINE__, "bad type given to typeCopy");
    }
  }
}
bool typeEqual(Type const *a, Type const *b) {
  if (a->kind != b->kind) return false;

  switch (a->kind) {
    case TK_KEYWORD: {
      return a->data.keyword.keyword == b->data.keyword.keyword;
    }
    case TK_QUALIFIED: {
      return a->data.qualified.constQual == b->data.qualified.constQual &&
             a->data.qualified.volatileQual == b->data.qualified.volatileQual &&
             typeEqual(a->data.qualified.base, b->data.qualified.base);
    }
    case TK_POINTER: {
      return typeEqual(a->data.pointer.base, b->data.pointer.base);
    }
    case TK_ARRAY: {
      return a->data.array.length == b->data.array.length &&
             typeEqual(a->data.array.type, b->data.array.type);
    }
    case TK_FUNPTR: {
      if (!typeEqual(a->data.funPtr.returnType, b->data.funPtr.returnType))
        return false;
      if (a->data.funPtr.argTypes.size != b->data.funPtr.argTypes.size)
        return false;
      for (size_t idx = 0; idx < a->data.funPtr.argTypes.size; ++idx) {
        if (!typeEqual(a->data.funPtr.argTypes.elements[idx],
                       b->data.funPtr.argTypes.elements[idx]))
          return false;
      }
      return true;
    }
    case TK_AGGREGATE: {
      if (a->data.aggregate.types.size != b->data.aggregate.types.size)
        return false;
      for (size_t idx = 0; idx < a->data.aggregate.types.size; ++idx) {
        if (!typeEqual(a->data.aggregate.types.elements[idx],
                       b->data.aggregate.types.elements[idx]))
          return false;
      }
      return true;
    }
    case TK_REFERENCE: {
      SymbolTableEntry *aEntry = a->data.reference.entry;
      SymbolTableEntry *bEntry = b->data.reference.entry;
      return aEntry == bEntry ||
             (aEntry->kind == SK_OPAQUE &&
              aEntry->data.opaqueType.definition == bEntry) ||
             (bEntry->kind == SK_OPAQUE &&
              aEntry == bEntry->data.opaqueType.definition) ||
             (aEntry->kind == SK_OPAQUE && bEntry->kind == SK_OPAQUE &&
              aEntry->data.opaqueType.definition ==
                  bEntry->data.opaqueType.definition);
    }
    default: {
      error(__FILE__, __LINE__, "bad type given to typeEqual");
    }
  }
}
bool typeImplicitlyConvertable(Type const *from, Type const *to) {
  return false;  // TODO
}
char *typeVectorToString(Vector const *v) {
  if (v->size == 0) {
    return strdup("");
  } else {
    char *base = typeToString(v->elements[0]);
    for (size_t idx = 1; idx < v->size; ++idx) {
      char *tmp = base;
      char *add = typeToString(v->elements[idx]);
      base = format("%s, %s", tmp, add);
      free(tmp);
      free(add);
    }
    return base;
  }
}
char *typeToString(Type const *t) {
  switch (t->kind) {
    case TK_KEYWORD: {
      switch (t->data.keyword.keyword) {
        case TK_VOID: {
          return strdup("void");
        }
        case TK_UBYTE: {
          return strdup("ubyte");
        }
        case TK_BYTE: {
          return strdup("byte");
        }
        case TK_CHAR: {
          return strdup("char");
        }
        case TK_USHORT: {
          return strdup("ushort");
        }
        case TK_SHORT: {
          return strdup("short");
        }
        case TK_UINT: {
          return strdup("uint");
        }
        case TK_INT: {
          return strdup("int");
        }
        case TK_WCHAR: {
          return strdup("wchar");
        }
        case TK_ULONG: {
          return strdup("ulong");
        }
        case TK_LONG: {
          return strdup("long");
        }
        case TK_FLOAT: {
          return strdup("float");
        }
        case TK_DOUBLE: {
          return strdup("double");
        }
        case TK_BOOL: {
          return strdup("bool");
        }
        default: {
          error(__FILE__, __LINE__, "invalid type keyword enum given");
        }
      }
    }
    case TK_QUALIFIED: {
      char *base = typeToString(t->data.qualified.base);
      char *retval;
      if (t->data.qualified.constQual && t->data.qualified.volatileQual)
        retval = format("%s volatile const", base);
      else if (t->data.qualified.constQual)
        retval = format("%s const", base);
      else  // at least one of const, volatile must be true
        retval = format("%s volatile", base);
      free(base);
      return retval;
    }
    case TK_POINTER: {
      char *base = typeToString(t->data.pointer.base);
      char *retval;
      if (base[strlen(base) - 1] == '*')
        retval = format("%s*", base);
      else
        retval = format("%s *", base);
      free(base);
      return retval;
    }
    case TK_ARRAY: {
      char *base = typeToString(t->data.array.type);
      char *retval = format("%s[%lu]", base, t->data.array.length);
      free(base);
      return retval;
    }
    case TK_FUNPTR: {
      char *returnType = typeToString(t->data.funPtr.returnType);
      char *args = typeVectorToString(&t->data.funPtr.argTypes);
      char *retval = format("%s(%s)", returnType, args);
      free(returnType);
      free(args);
      return retval;
    }
    case TK_AGGREGATE: {
      char *elms = typeVectorToString(&t->data.aggregate.types);
      char *retval = format("{%s}", elms);
      free(elms);
      return retval;
    }
    case TK_REFERENCE: {
      return strdup(t->data.reference.id);
    }
    default: {
      error(__FILE__, __LINE__, "invalid typekind enum encountered");
    }
  }
}
void typeFree(Type *t) {
  if (t == NULL) return;

  switch (t->kind) {
    case TK_QUALIFIED: {
      typeFree(t->data.qualified.base);
      break;
    }
    case TK_POINTER: {
      typeFree(t->data.pointer.base);
      break;
    }
    case TK_ARRAY: {
      typeFree(t->data.array.type);
      break;
    }
    case TK_FUNPTR: {
      vectorUninit(&t->data.funPtr.argTypes, (void (*)(void *))typeFree);
      typeFree(t->data.funPtr.returnType);
      break;
    }
    case TK_AGGREGATE: {
      vectorUninit(&t->data.aggregate.types, (void (*)(void *))typeFree);
      break;
    }
    case TK_REFERENCE: {
      free(t->data.reference.id);
      break;
    }
    default: {
      break;  // nothing to do
    }
  }
  free(t);
}

void typeVectorFree(Vector *v) {
  vectorUninit(v, (void (*)(void *))typeFree);
  free(v);
}