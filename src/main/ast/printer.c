// Copyright 2019 Justin Hu
//
// This file is part of the T Language Compiler.

#include "ast/printer.h"

#include <stdio.h>
#include <stdlib.h>

void printNodeStructure(Node *node) {
  if (node == NULL) return;

  switch (node->type) {
    case TYPE_PROGRAM: {
      printf("PROGRAM(");
      printNodeStructure(node->data.program.module);
      for (size_t idx = 0; idx < node->data.program.numImports; idx++) {
        printf(" ");
        printNodeStructure(node->data.program.imports[idx]);
      }
      for (size_t idx = 0; idx < node->data.program.numBodies; idx++) {
        printf(" ");
        printNodeStructure(node->data.program.bodies[idx]);
      }
      printf(")");
      break;
    }
    case TYPE_MODULE: {
      printf("MODULE(");
      printNodeStructure(node->data.module.id);
      printf(")");
      break;
    }
    case TYPE_IMPORT: {
      printf("IMPORT(");
      printNodeStructure(node->data.import.id);
      printf(")");
      break;
    }
    case TYPE_FUNDECL: {
      printf("FUNDECL(");
      printNodeStructure(node->data.funDecl.returnType);
      printf(" ");
      printNodeStructure(node->data.funDecl.id);
      for (size_t idx = 0; idx < node->data.funDecl.numParamTypes; idx++) {
        printf(" ");
        printNodeStructure(node->data.funDecl.paramTypes[idx]);
      }
      printf(")");
      break;
    }
    case TYPE_VARDECL: {
      printf("VARDECL(");
      printNodeStructure(node->data.varDecl.type);
      for (size_t idx = 0; idx < node->data.varDecl.numIds; idx++) {
        printf(" ");
        printNodeStructure(node->data.varDecl.ids[idx]);
      }
      printf(")");
      break;
    }
    case TYPE_STRUCTDECL: {
      printf("STRUCTDECL(");
      printNodeStructure(node->data.structDecl.id);
      for (size_t idx = 0; idx < node->data.structDecl.numDecls; idx++) {
        printf(" ");
        printNodeStructure(node->data.structDecl.decls[idx]);
      }
      printf(")");
      break;
    }
    case TYPE_UNIONDECL: {
      printf("UNIONDECL(");
      printNodeStructure(node->data.unionDecl.id);
      for (size_t idx = 0; idx < node->data.unionDecl.numOpts; idx++) {
        printf(" ");
        printNodeStructure(node->data.unionDecl.opts[idx]);
      }
      printf(")");
      break;
    }
    case TYPE_TYPEDEFDECL: {
      printf("TYPEDEFDECL(");
      printNodeStructure(node->data.typedefDecl.type);
      printf(" ");
      printNodeStructure(node->data.typedefDecl.id);
      printf(")");
      break;
    }
    case TYPE_FUNCTION: {
      printf("FUNCTION(");
      printNodeStructure(node->data.function.returnType);
      printf(" ");
      printNodeStructure(node->data.function.id);
      for (size_t idx = 0; idx < node->data.function.numFormals; idx++) {
        printf(" FORMAL(");
        printNodeStructure(node->data.function.formalTypes[idx]);
        if (node->data.function.formalIds[idx] != NULL) {
          printf(" ");
          printNodeStructure(node->data.function.formalIds[idx]);
        }
        printf(")");
      }
      printf(" ");
      printNodeStructure(node->data.function.body);
      printf(")");
      break;
    }
    case TYPE_COMPOUNDSTMT: {
      printf("COMPOUNDSTMT(");
      for (size_t idx = 0; idx < node->data.compoundStmt.numStatements; idx++) {
        if (idx != 0) printf(" ");
        printNodeStructure(node->data.compoundStmt.statements[idx]);
      }
      printf(")");
      break;
    }
    case TYPE_IFSTMT: {
      printf("IFSTMT(");
      printNodeStructure(node->data.ifStmt.condition);
      printf(" ");
      printNodeStructure(node->data.ifStmt.thenStmt);
      if (node->data.ifStmt.elseStmt != NULL) {
        printf(" ");
        printNodeStructure(node->data.ifStmt.elseStmt);
      }
      printf(")");
      break;
    }
    case TYPE_WHILESTMT: {
      printf("WHILESTMT(");
      printNodeStructure(node->data.whileStmt.condition);
      printf(" ");
      printNodeStructure(node->data.whileStmt.body);
      printf(")");
      break;
    }
    case TYPE_DOWHILESTMT: {
      printf("DOWHILESTMT(");
      printNodeStructure(node->data.doWhileStmt.body);
      printf(" ");
      printNodeStructure(node->data.doWhileStmt.condition);
      printf(")");
      break;
    }
    case TYPE_FORSTMT: {
      printf("FORSTMT(");
      printNodeStructure(node->data.forStmt.initialize);
      printf(" ");
      printNodeStructure(node->data.forStmt.condition);
      printf(" ");
      printNodeStructure(node->data.forStmt.update);
      printf(" ");
      printNodeStructure(node->data.forStmt.body);
      printf(")");
      break;
    }
    case TYPE_SWITCHSTMT: {
      printf("SWITCHSTMT(");
      printNodeStructure(node->data.switchStmt.onWhat);
      printf(" ");
      for (size_t idx = 0; idx < node->data.switchStmt.numCases; idx++) {
        printf(" ");
        printNodeStructure(node->data.switchStmt.cases[idx]);
      }
      printf(")");
      break;
    }
    case TYPE_NUMCASE: {
      printf("NUMCASE(");
      printNodeStructure(node->data.numCase.constVal);
      printf(" ");
      printNodeStructure(node->data.numCase.body);
      printf(")");
      break;
    }
    case TYPE_DEFAULTCASE: {
      printf("DEFAULTCASE(");
      printNodeStructure(node->data.defaultCase.body);
      printf(")");
      break;
    }
    case TYPE_BREAKSTMT: {
      printf("BREAKSTMT");
      break;
    }
    case TYPE_CONTINUESTMT: {
      printf("CONTINUESTMT");
      break;
    }
    case TYPE_RETURNSTMT: {
      printf("RETURNSTMT");
      if (node->data.returnStmt.value != NULL) {
        printf("(");
        printNodeStructure(node->data.returnStmt.value);
        printf(")");
      }
      break;
    }
    case TYPE_VARDECLSTMT: {
      printf("VARDECLSTMT(");
      printNodeStructure(node->data.varDeclStmt.type);
      printf(" ");
      for (size_t idx = 0; idx < node->data.varDeclStmt.numIds; idx++) {
        printf(" DECL(");
        printNodeStructure(node->data.varDeclStmt.ids[idx]);
        if (node->data.varDeclStmt.values[idx] != NULL) {
          printf(" ");
          printNodeStructure(node->data.varDeclStmt.values[idx]);
        }
        printf(")");
      }
      printf(")");
      break;
    }
    case TYPE_ASMSTMT: {
      printf("ASMSTMT(");
      printNodeStructure(node->data.asmStmt.assembly);
      printf(")");
      break;
    }
    case TYPE_EXPRESSIONSTMT: {
      printf("EXPRESSIONSTMT(");
      printNodeStructure(node->data.expressionStmt.expression);
      printf(")");
      break;
    }
    case TYPE_NULLSTMT: {
      printf("NULLSTMT");
      break;
    }
    case TYPE_SEQEXP: {
      printf("SEQEXP(");
      printNodeStructure(node->data.seqExp.first);
      printf(" ");
      printNodeStructure(node->data.seqExp.second);
      printf(")");
      break;
    }
    case TYPE_BINOPEXP: {
      printf("BINOPEXP(");
      switch (node->data.binopExp.op) {
        case OP_ASSIGN: {
          printf("ASSIGN");
          break;
        }
        case OP_MULASSIGN: {
          printf("MULASSIGN");
          break;
        }
        case OP_DIVASSIGN: {
          printf("DVIASSIGN");
          break;
        }
        case OP_MODASSIGN: {
          printf("MODASSIGN");
          break;
        }
        case OP_ADDASSIGN: {
          printf("ADDASSIGN");
          break;
        }
        case OP_SUBASSIGN: {
          printf("SUBASSIGN");
          break;
        }
        case OP_LSHIFTASSIGN: {
          printf("LSHIFTASSIGN");
          break;
        }
        case OP_LRSHIFTASSIGN: {
          printf("LRSHIFTASSIGN");
          break;
        }
        case OP_ARSHIFTASSIGN: {
          printf("ARSHIFTASSIGN");
          break;
        }
        case OP_BITANDASSIGN: {
          printf("BITANDASSIGN");
          break;
        }
        case OP_BITXORASSIGN: {
          printf("BITXORASSIGN");
          break;
        }
        case OP_BITORASSIGN: {
          printf("BITORASSIGN");
          break;
        }
        case OP_BITAND: {
          printf("BITAND");
          break;
        }
        case OP_BITOR: {
          printf("BITOR");
          break;
        }
        case OP_BITXOR: {
          printf("BITXOR");
          break;
        }
        case OP_SPACESHIP: {
          printf("SPACESHIP");
          break;
        }
        case OP_LSHIFT: {
          printf("LSHIFT");
          break;
        }
        case OP_LRSHIFT: {
          printf("LRSHIFT");
          break;
        }
        case OP_ARSHIFT: {
          printf("ARSHIFT");
          break;
        }
        case OP_ADD: {
          printf("ADD");
          break;
        }
        case OP_SUB: {
          printf("SUB");
          break;
        }
        case OP_MUL: {
          printf("MUL");
          break;
        }
        case OP_DIV: {
          printf("DIV");
          break;
        }
        case OP_MOD: {
          printf("MOD");
          break;
        }
        case OP_ARRAYACCESS: {
          printf("ARRAYACCESS");
          break;
        }
      }
      printf(" ");
      printNodeStructure(node->data.binopExp.lhs);
      printf(" ");
      printNodeStructure(node->data.binopExp.rhs);
      printf(")");
      break;
    }
    case TYPE_UNOPEXP: {
      printf("UNOPEXP(");
      switch (node->data.unOpExp.op) {
        case OP_DEREF: {
          printf("DEREF");
          break;
        }
        case OP_ADDROF: {
          printf("ADDROF");
          break;
        }
        case OP_PREINC: {
          printf("PREINC");
          break;
        }
        case OP_PREDEC: {
          printf("PREDEC");
          break;
        }
        case OP_UPLUS: {
          printf("UPLUS");
          break;
        }
        case OP_NEG: {
          printf("NEG");
          break;
        }
        case OP_LNOT: {
          printf("LNOT");
          break;
        }
        case OP_BITNOT: {
          printf("BITNOT");
          break;
        }
        case OP_POSTINC: {
          printf("POSTINC");
          break;
        }
        case OP_POSTDEC: {
          printf("POSTDEC");
          break;
        }
      }
      printf(" ");
      printNodeStructure(node->data.unOpExp.target);
      printf(")");
      break;
    }
    case TYPE_COMPOPEXP: {
      printf("COMPOPEXP(");
      switch (node->data.compOpExp.op) {
        case OP_EQ: {
          printf("EQ");
          break;
        }
        case OP_NEQ: {
          printf("NEQ");
          break;
        }
        case OP_LT: {
          printf("LT");
          break;
        }
        case OP_GT: {
          printf("GT");
          break;
        }
        case OP_LTEQ: {
          printf("LTEQ");
          break;
        }
        case OP_GTEQ: {
          printf("GTEQ");
          break;
        }
      }
      printf(" ");
      printNodeStructure(node->data.compOpExp.lhs);
      printf(" ");
      printNodeStructure(node->data.compOpExp.rhs);
      printf(")");
      break;
    }
    case TYPE_LANDASSIGNEXP: {
      printf("LANDASSIGNEXP(");
      printNodeStructure(node->data.landAssignExp.lhs);
      printf(" ");
      printNodeStructure(node->data.landAssignExp.rhs);
      printf(")");
      break;
    }
    case TYPE_LORASSIGNEXP: {
      printf("LORASSIGNEXP(");
      printNodeStructure(node->data.lorAssignExp.lhs);
      printf(" ");
      printNodeStructure(node->data.lorAssignExp.rhs);
      printf(")");
      break;
    }
    case TYPE_TERNARYEXP: {
      printf("TERNARYEXP(");
      printNodeStructure(node->data.ternaryExp.condition);
      printf(" ");
      printNodeStructure(node->data.ternaryExp.thenExp);
      printf(" ");
      printNodeStructure(node->data.ternaryExp.elseExp);
      printf(")");
      break;
    }
    case TYPE_LANDEXP: {
      printf("LANDEXP(");
      printNodeStructure(node->data.landExp.lhs);
      printf(" ");
      printNodeStructure(node->data.landExp.rhs);
      printf(")");
      break;
    }
    case TYPE_LOREXP: {
      printf("LOREXP(");
      printNodeStructure(node->data.lorExp.lhs);
      printf(" ");
      printNodeStructure(node->data.lorExp.rhs);
      printf(")");
      break;
    }
    case TYPE_STRUCTACCESSEXP: {
      printf("STRUCTACCESSEXP(");
      printNodeStructure(node->data.structAccessExp.base);
      printf(" ");
      printNodeStructure(node->data.structAccessExp.element);
      printf(")");
      break;
    }
    case TYPE_STRUCTPTRACCESSEXP: {
      printf("STRUCTPTRACCESSEXP(");
      printNodeStructure(node->data.structPtrAccessExp.base);
      printf(" ");
      printNodeStructure(node->data.structPtrAccessExp.element);
      printf(")");
      break;
    }
    case TYPE_FNCALLEXP: {
      printf("FNCALLEXP(");
      printNodeStructure(node->data.fnCallExp.who);
      for (size_t idx = 0; idx < node->data.fnCallExp.numArgs; idx++) {
        printf(" ");
        printNodeStructure(node->data.fnCallExp.args[idx]);
      }
      printf(")");
      break;
    }
    case TYPE_IDEXP: {
      printf("ID(%s)", node->data.idExp.id);
      break;
    }
    case TYPE_CONSTEXP: {
      switch (node->data.constExp.type) {
        case CTYPE_UBYTE: {
          printf("CONST(%hhu)", node->data.constExp.value.ubyteVal);
          break;
        }
        case CTYPE_BYTE: {
          printf("CONST(%hhd)", node->data.constExp.value.byteVal);
          break;
        }
        case CTYPE_UINT: {
          printf("CONST(%u)", node->data.constExp.value.uintVal);
          break;
        }
        case CTYPE_INT: {
          printf("CONST(%d)", node->data.constExp.value.intVal);
          break;
        }
        case CTYPE_ULONG: {
          printf("CONST(%lu)", node->data.constExp.value.ulongVal);
          break;
        }
        case CTYPE_LONG: {
          printf("CONST(%ld)", node->data.constExp.value.longVal);
          break;
        }
        case CTYPE_FLOAT: {
          printf("CONST(%e)",
                 (double)*(float *)&node->data.constExp.value.floatBits);
          break;
        }
        case CTYPE_DOUBLE: {
          printf("CONST(%e)", *(double *)&node->data.constExp.value.doubleBits);
          break;
        }
        case CTYPE_CSTRING: {
          printf("CONST(%s)", node->data.constExp.value.cstringVal);
          break;
        }
        case CTYPE_CHAR: {
          printf("CONST(%c)", node->data.constExp.value.charVal);
          break;
        }
        case CTYPE_WCSTRING: {
          printf("CONST(%ls)",
                 (wchar_t *)node->data.constExp.value.wcstringVal);
          break;
        }
        case CTYPE_WCHAR: {
          printf("CONST(%lc)", (wchar_t)node->data.constExp.value.wcharVal);
          break;
        }
        case CTYPE_BOOL: {
          printf(node->data.constExp.value.boolVal ? "CONST(true)"
                                                   : "CONST(false)");
        }
      }
      break;
    }
    case TYPE_AGGREGATEINITEXP: {
      printf("AGGREGATEINITEXP(");
      for (size_t idx = 0; idx < node->data.aggregateInitExp.numElements;
           idx++) {
        if (idx != 0) printf(" ");
        printNodeStructure(node->data.aggregateInitExp.elements[idx]);
      }
      printf(")");
      break;
    }
    case TYPE_CASTEXP: {
      printf("CASTEXP(");
      printNodeStructure(node->data.castExp.toWhat);
      printf(" ");
      printNodeStructure(node->data.castExp.target);
      printf(")");
      break;
    }
    case TYPE_SIZEOFTYPEEXP: {
      printf("SIZEOFTYPEEXP(");
      printNodeStructure(node->data.sizeofTypeExp.target);
      printf(")");
      break;
    }
    case TYPE_SIZEOFEXPEXP: {
      printf("SIZEOFEXPEXP(");
      printNodeStructure(node->data.sizeofExpExp.target);
      printf(")");
      break;
    }
    case TYPE_KEYWORDTYPE: {
      switch (node->data.keywordType.type) {
        case TYPEKWD_VOID: {
          printf("KEYWORDTYPE(void)");
          break;
        }
        case TYPEKWD_UBYTE: {
          printf("KEYWORDTYPE(ubyte)");
          break;
        }
        case TYPEKWD_BYTE: {
          printf("KEYWORDTYPE(byte)");
          break;
        }
        case TYPEKWD_UINT: {
          printf("KEYWORDTYPE(uint)");
          break;
        }
        case TYPEKWD_INT: {
          printf("KEYWORDTYPE(int)");
          break;
        }
        case TYPEKWD_ULONG: {
          printf("KEYWORDTYPE(ulong)");
          break;
        }
        case TYPEKWD_LONG: {
          printf("KEYWORDTYPE(long)");
          break;
        }
        case TYPEKWD_FLOAT: {
          printf("KEYWORDTYPE(float)");
          break;
        }
        case TYPEKWD_DOUBLE: {
          printf("KEYWORDTYPE(double)");
          break;
        }
        case TYPEKWD_BOOL: {
          printf("KEYWORDTYPE(bool)");
          break;
        }
      }
      break;
    }
    case TYPE_IDTYPE: {
      printf("IDTYPE(%s)", node->data.idType.id);
      break;
    }
    case TYPE_CONSTTYPE: {
      printf("CONSTTYPE(");
      printNodeStructure(node->data.constType.target);
      printf(")");
      break;
    }
    case TYPE_ARRAYTYPE: {
      printf("ARRAYTYPE(");
      printNodeStructure(node->data.arrayType.element);
      printf(" ");
      printNodeStructure(node->data.arrayType.size);
      printf(")");
      break;
    }
    case TYPE_PTRTYPE: {
      printf("PTRTYPE(");
      printNodeStructure(node->data.ptrType.target);
      printf(")");
      break;
    }
    case TYPE_FNPTRTYPE: {
      printf("FNPTRTYPE(");
      printNodeStructure(node->data.fnPtrType.returnType);
      for (size_t idx = 0; idx < node->data.fnPtrType.numArgTypes; idx++) {
        printf(" ");
        printNodeStructure(node->data.fnPtrType.argTypes[idx]);
      }
      printf(")");
      break;
    }
    case TYPE_ID: {
      printf("ID(%s)", node->data.id.id);
      break;
    }
  }
}

void printNode(Node *node) {
  if (node == NULL) return;

  switch (node->type) {
    case TYPE_PROGRAM: {
      printNode(node->data.program.module);
      printf("\n");
      for (size_t idx = 0; idx < node->data.program.numImports; idx++) {
        printNode(node->data.program.imports[idx]);
      }
      if (node->data.program.numImports != 0) printf("\n");
      for (size_t idx = 0; idx < node->data.program.numBodies; idx++) {
        printNode(node->data.program.bodies[idx]);
      }
      break;
    }
    case TYPE_MODULE: {
      printf("module ");
      printNode(node->data.module.id);
      printf(";\n");
      break;
    }
    case TYPE_IMPORT: {
      printf("using ");
      printNode(node->data.import.id);
      printf(";\n");
      break;
    }
    case TYPE_FUNDECL: {
      printNode(node->data.funDecl.returnType);
      printf(" ");
      printNode(node->data.funDecl.id);
      printf("(");
      for (size_t idx = 0; idx < node->data.funDecl.numParamTypes; idx++) {
        printNode(node->data.funDecl.paramTypes[idx]);
        if (idx != node->data.funDecl.numParamTypes - 1) {
          printf(", ");
        }
      }
      printf(");\n");
      break;
    }
    case TYPE_VARDECL: {
      printNode(node->data.varDecl.type);
      printf(" ");
      for (size_t idx = 0; idx < node->data.varDecl.numIds; idx++) {
        printNode(node->data.varDecl.ids[idx]);
        if (idx != node->data.varDecl.numIds - 1) {
          printf(", ");
        }
        printf(";\n");
      }
      break;
    }
    case TYPE_STRUCTDECL: {
      printf("struct ");
      printNode(node->data.structDecl.id);
      printf(" {\n");
      for (size_t idx = 0; idx < node->data.structDecl.numDecls; idx++) {
        printNode(node->data.structDecl.decls[idx]);
        if (idx != node->data.structDecl.numDecls - 1) {
          printf("\n");
        }
      }
      printf("};\n");
      break;
    }
    case TYPE_UNIONDECL: {
      printf("union ");
      printNode(node->data.unionDecl.id);
      printf(" {\n");
      for (size_t idx = 0; idx < node->data.unionDecl.numOpts; idx++) {
        printNode(node->data.unionDecl.opts[idx]);
        if (idx != node->data.unionDecl.numOpts - 1) {
          printf("\n");
        }
      }
      printf("};\n");
      break;
    }
    case TYPE_TYPEDEFDECL: {
      printf("typedef ");
      printNode(node->data.typedefDecl.type);
      printf(" ");
      printNode(node->data.typedefDecl.id);
      printf(";");
      break;
    }
    case TYPE_FUNCTION: {
      printNode(node->data.function.returnType);
      printf(" ");
      printNode(node->data.function.id);
      printf("(");
      for (size_t idx = 0; idx < node->data.function.numFormals; idx++) {
        printNode(node->data.function.formalTypes[idx]);
        if (node->data.function.formalIds[idx] != NULL) {
          printf(" ");
          printNode(node->data.function.formalIds[idx]);
        }
        if (idx != node->data.function.numFormals - 1) {
          printf(", ");
        }
      }
      printf(") ");
      printNode(node->data.function.body);
      printf("\n");
      break;
    }
    case TYPE_COMPOUNDSTMT: {
      printf("{\n");
      for (size_t idx = 0; idx < node->data.compoundStmt.numStatements; idx++) {
        printNode(node->data.compoundStmt.statements[idx]);
      }
      printf("}");
      break;
    }
    case TYPE_IFSTMT: {
      printf("if (");
      printNode(node->data.ifStmt.condition);
      printf(") ");
      printNode(node->data.ifStmt.thenStmt);
      if (node->data.ifStmt.elseStmt != NULL) {
        printf("else ");
        printNode(node->data.ifStmt.elseStmt);
      }
      printf("\n");
      break;
    }
    case TYPE_WHILESTMT: {
      printf("while (");
      printNode(node->data.whileStmt.condition);
      printf(") ");
      printNode(node->data.whileStmt.body);
      printf("\n");
      break;
    }
    case TYPE_DOWHILESTMT: {
      printf("do ");
      printNode(node->data.doWhileStmt.body);
      printf("\nwhile (");
      printNode(node->data.doWhileStmt.condition);
      printf(")\n");
      break;
    }
    case TYPE_FORSTMT: {
      printf("for (");
      printNode(node->data.forStmt.initialize);
      printf(" ");
      printNode(node->data.forStmt.condition);
      printf("; ");
      printNode(node->data.forStmt.update);
      printf(") ");
      printNode(node->data.forStmt.body);
      printf("\n");
      break;
    }
    case TYPE_SWITCHSTMT: {
      printf("switch (");
      printNode(node->data.switchStmt.onWhat);
      printf(") {\n");
      for (size_t idx = 0; idx < node->data.switchStmt.numCases; idx++) {
        printNode(node->data.switchStmt.cases[idx]);
      }
      printf("}\n");
      break;
    }
    case TYPE_NUMCASE: {
      printf("case ");
      printNode(node->data.numCase.constVal);
      printf(": ");
      printNode(node->data.numCase.body);
      printf("\n");
      break;
    }
    case TYPE_DEFAULTCASE: {
      printf("default: ");
      printNode(node->data.defaultCase.body);
      printf("\n");
      break;
    }
    case TYPE_BREAKSTMT: {
      printf("break;\n");
      break;
    }
    case TYPE_CONTINUESTMT: {
      printf("continue;\n");
      break;
    }
    case TYPE_RETURNSTMT: {
      printf("return");
      if (node->data.returnStmt.value != NULL) {
        printf(" ");
        printNode(node->data.returnStmt.value);
      }
      printf(";");
      break;
    }
    case TYPE_VARDECLSTMT: {
      printNode(node->data.varDeclStmt.type);
      printf(" ");
      for (size_t idx = 0; idx < node->data.varDeclStmt.numIds; idx++) {
        printNode(node->data.varDeclStmt.ids[idx]);
        if (node->data.varDeclStmt.values[idx] != NULL) {
          printf(" = ");
          printNode(node->data.varDeclStmt.values[idx]);
        }
        if (idx != node->data.varDeclStmt.numIds - 1) {
          printf(", ");
        }
      }
      printf(";\n");
      break;
    }
    case TYPE_ASMSTMT: {
      printf("asm ");
      printNode(node->data.asmStmt.assembly);
      printf(";\n");
      break;
    }
    case TYPE_EXPRESSIONSTMT: {
      printNode(node->data.expressionStmt.expression);
      printf(";\n");
      break;
    }
    case TYPE_NULLSTMT: {
      printf(";\n");
      break;
    }
    case TYPE_SEQEXP: {
      printf("(");
      printNode(node->data.seqExp.first);
      printf(", ");
      printNode(node->data.seqExp.second);
      printf(")");
      break;
    }
    case TYPE_BINOPEXP: {
      if (node->data.binopExp.op == OP_ARRAYACCESS) {
        printf("(");
        printNode(node->data.binopExp.lhs);
        printf("[");
        printNode(node->data.binopExp.rhs);
        printf("])");
        break;
      }
      printf("(");
      printNode(node->data.binopExp.lhs);
      switch (node->data.binopExp.op) {
        case OP_ASSIGN: {
          printf(" = ");
          break;
        }
        case OP_MULASSIGN: {
          printf(" *= ");
          break;
        }
        case OP_DIVASSIGN: {
          printf(" /= ");
          break;
        }
        case OP_MODASSIGN: {
          printf(" %%= ");
          break;
        }
        case OP_ADDASSIGN: {
          printf(" += ");
          break;
        }
        case OP_SUBASSIGN: {
          printf(" -= ");
          break;
        }
        case OP_LSHIFTASSIGN: {
          printf(" <<= ");
          break;
        }
        case OP_LRSHIFTASSIGN: {
          printf(" >>= ");
          break;
        }
        case OP_ARSHIFTASSIGN: {
          printf(" >>>= ");
          break;
        }
        case OP_BITANDASSIGN: {
          printf(" &= ");
          break;
        }
        case OP_BITXORASSIGN: {
          printf(" ^= ");
          break;
        }
        case OP_BITORASSIGN: {
          printf(" |= ");
          break;
        }
        case OP_BITAND: {
          printf(" & ");
          break;
        }
        case OP_BITOR: {
          printf(" | ");
          break;
        }
        case OP_BITXOR: {
          printf(" ^ ");
          break;
        }
        case OP_SPACESHIP: {
          printf(" <=> ");
          break;
        }
        case OP_LSHIFT: {
          printf(" << ");
          break;
        }
        case OP_LRSHIFT: {
          printf(" >> ");
          break;
        }
        case OP_ARSHIFT: {
          printf(" >>> ");
          break;
        }
        case OP_ADD: {
          printf(" + ");
          break;
        }
        case OP_SUB: {
          printf(" - ");
          break;
        }
        case OP_MUL: {
          printf(" * ");
          break;
        }
        case OP_DIV: {
          printf(" / ");
          break;
        }
        case OP_MOD: {
          printf(" %% ");
          break;
        }
        case OP_ARRAYACCESS:
          abort();
      }
      printNode(node->data.binopExp.rhs);
      printf(")");
      break;
    }
    case TYPE_UNOPEXP: {
      printf("(");
      switch (node->data.unOpExp.op) {
        case OP_DEREF: {
          printf("*");
          break;
        }
        case OP_ADDROF: {
          printf("&");
          break;
        }
        case OP_PREINC: {
          printf("++");
          break;
        }
        case OP_PREDEC: {
          printf("--");
          break;
        }
        case OP_UPLUS: {
          printf("+");
          break;
        }
        case OP_NEG: {
          printf("-");
          break;
        }
        case OP_LNOT: {
          printf("!");
          break;
        }
        case OP_BITNOT: {
          printf("~");
          break;
        }
        case OP_POSTINC:
        case OP_POSTDEC:
          break;
      }
      printNode(node->data.unOpExp.target);
      switch (node->data.unOpExp.op) {
        case OP_POSTINC: {
          printf("++");
          break;
        }
        case OP_POSTDEC: {
          printf("--");
          break;
        }
        case OP_DEREF:
        case OP_ADDROF:
        case OP_PREINC:
        case OP_PREDEC:
        case OP_UPLUS:
        case OP_NEG:
        case OP_LNOT:
        case OP_BITNOT:
          break;
      }
      break;
    }
    case TYPE_COMPOPEXP: {
      printf("(");
      printNode(node->data.compOpExp.lhs);
      switch (node->data.compOpExp.op) {
        case OP_EQ: {
          printf(" == ");
          break;
        }
        case OP_NEQ: {
          printf(" != ");
          break;
        }
        case OP_LT: {
          printf(" < ");
          break;
        }
        case OP_GT: {
          printf(" > ");
          break;
        }
        case OP_LTEQ: {
          printf(" <= ");
          break;
        }
        case OP_GTEQ: {
          printf(" >= ");
          break;
        }
      }
      printNode(node->data.compOpExp.rhs);
      printf(")");
      break;
    }
    case TYPE_LANDASSIGNEXP: {
      printf("(");
      printNode(node->data.landAssignExp.lhs);
      printf(" &&= ");
      printNode(node->data.landAssignExp.rhs);
      printf(")");
      break;
    }
    case TYPE_LORASSIGNEXP: {
      printf("(");
      printNode(node->data.lorAssignExp.lhs);
      printf(" ||= ");
      printNode(node->data.lorAssignExp.rhs);
      printf(")");
      break;
    }
    case TYPE_TERNARYEXP: {
      printf("(");
      printNode(node->data.ternaryExp.condition);
      printf(" ? ");
      printNode(node->data.ternaryExp.thenExp);
      printf(" : ");
      printNode(node->data.ternaryExp.elseExp);
      printf(")");
      break;
    }
    case TYPE_LANDEXP: {
      printf("(");
      printNode(node->data.landExp.lhs);
      printf(" && ");
      printNode(node->data.landExp.rhs);
      printf(")");
      break;
    }
    case TYPE_LOREXP: {
      printf("(");
      printNode(node->data.lorExp.lhs);
      printf(" || ");
      printNode(node->data.lorExp.rhs);
      printf(")");
      break;
    }
    case TYPE_STRUCTACCESSEXP: {
      printf("(");
      printNode(node->data.structAccessExp.base);
      printf(".");
      printNode(node->data.structAccessExp.element);
      printf(")");
      break;
    }
    case TYPE_STRUCTPTRACCESSEXP: {
      printf("(");
      printNode(node->data.structPtrAccessExp.base);
      printf("->");
      printNode(node->data.structPtrAccessExp.element);
      printf(")");
      break;
    }
    case TYPE_FNCALLEXP: {
      printf("(");
      printNode(node->data.fnCallExp.who);
      printf("(");
      for (size_t idx = 0; idx < node->data.fnCallExp.numArgs; idx++) {
        printNode(node->data.fnCallExp.args[idx]);
        if (idx != node->data.fnCallExp.numArgs - 1) {
          printf(", ");
        }
      }
      printf("))");
      break;
    }
    case TYPE_IDEXP: {
      printf("%s", node->data.idExp.id);
      break;
    }
    case TYPE_CONSTEXP: {
      switch (node->data.constExp.type) {
        case CTYPE_UBYTE: {
          printf("%hhu", node->data.constExp.value.ubyteVal);
          break;
        }
        case CTYPE_BYTE: {
          printf("%hhd", node->data.constExp.value.byteVal);
          break;
        }
        case CTYPE_UINT: {
          printf("%u", node->data.constExp.value.uintVal);
          break;
        }
        case CTYPE_INT: {
          printf("%d", node->data.constExp.value.intVal);
          break;
        }
        case CTYPE_ULONG: {
          printf("%lu", node->data.constExp.value.ulongVal);
          break;
        }
        case CTYPE_LONG: {
          printf("%ld", node->data.constExp.value.longVal);
          break;
        }
        case CTYPE_FLOAT: {
          printf("%e", (double)*(float *)&node->data.constExp.value.floatBits);
          break;
        }
        case CTYPE_DOUBLE: {
          printf("%e", *(double *)&node->data.constExp.value.doubleBits);
          break;
        }
        case CTYPE_CSTRING: {
          printf("%s", node->data.constExp.value.cstringVal);
          break;
        }
        case CTYPE_CHAR: {
          printf("%c", node->data.constExp.value.charVal);
          break;
        }
        case CTYPE_WCSTRING: {
          printf("%ls", (wchar_t *)node->data.constExp.value.wcstringVal);
          break;
        }
        case CTYPE_WCHAR: {
          printf("%lc", (wchar_t)node->data.constExp.value.wcharVal);
          break;
        }
        case CTYPE_BOOL: {
          printf(node->data.constExp.value.boolVal ? "true" : "false");
        }
      }
      break;
    }
    case TYPE_AGGREGATEINITEXP: {
      printf("<");
      for (size_t idx = 0; idx < node->data.aggregateInitExp.numElements;
           idx++) {
        printNode(node->data.aggregateInitExp.elements[idx]);
        if (idx != node->data.aggregateInitExp.numElements - 1) {
          printf(", ");
        }
      }
      printf(">");
      break;
    }
    case TYPE_CASTEXP: {
      printf("cast[");
      printNode(node->data.castExp.toWhat);
      printf("](");
      printNode(node->data.castExp.target);
      printf(")");
      break;
    }
    case TYPE_SIZEOFTYPEEXP: {
      printf("sizeof(");
      printNode(node->data.sizeofTypeExp.target);
      printf(")");
      break;
    }
    case TYPE_SIZEOFEXPEXP: {
      printf("sizeof(");
      printNode(node->data.sizeofExpExp.target);
      printf(")");
      break;
    }
    case TYPE_KEYWORDTYPE: {
      switch (node->data.keywordType.type) {
        case TYPEKWD_VOID: {
          printf("void");
          break;
        }
        case TYPEKWD_UBYTE: {
          printf("ubyte");
          break;
        }
        case TYPEKWD_BYTE: {
          printf("byte");
          break;
        }
        case TYPEKWD_UINT: {
          printf("uint");
          break;
        }
        case TYPEKWD_INT: {
          printf("int");
          break;
        }
        case TYPEKWD_ULONG: {
          printf("ulong");
          break;
        }
        case TYPEKWD_LONG: {
          printf("long");
          break;
        }
        case TYPEKWD_FLOAT: {
          printf("float");
          break;
        }
        case TYPEKWD_DOUBLE: {
          printf("double");
          break;
        }
        case TYPEKWD_BOOL: {
          printf("bool");
          break;
        }
      }
      break;
    }
    case TYPE_IDTYPE: {
      printf("%s", node->data.idType.id);
      break;
    }
    case TYPE_CONSTTYPE: {
      printNode(node->data.constType.target);
      printf(" const");
      break;
    }
    case TYPE_ARRAYTYPE: {
      printNode(node->data.arrayType.element);
      printf("[");
      printNode(node->data.arrayType.size);
      printf("]");
      break;
    }
    case TYPE_PTRTYPE: {
      printNode(node->data.ptrType.target);
      printf("*");
      break;
    }
    case TYPE_FNPTRTYPE: {
      printNode(node->data.fnPtrType.returnType);
      printf("(");
      for (size_t idx = 0; idx < node->data.fnPtrType.numArgTypes; idx++) {
        printNode(node->data.fnPtrType.argTypes[idx]);
        if (idx != node->data.fnPtrType.numArgTypes - 1) {
          printf(", ");
        }
      }
      printf(")");
      break;
    }
    case TYPE_ID: {
      printf("%s", node->data.id.id);
      break;
    }
  }
}