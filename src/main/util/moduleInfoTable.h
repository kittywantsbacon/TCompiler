// Copyright 2019 Justin Hu
//
// This file is part of the T Language Compiler.

// Information attached to modules created during the dependency-finding phase

#ifndef TLC_UTIL_MODULEINFOTABLE_H_
#define TLC_UTIL_MODULEINFOTABLE_H_

#include "ast/ast.h"
#include "util/hashMap.h"

// holds the module name and its dependencies - POD object
typedef struct {
  const char *moduleName;
  size_t numDependencies;
  const char **dependencyNames;
} ModuleInfo;

// ctor
ModuleInfo *moduleInfoCreate(const char *moduleName, size_t numDependencies,
                             const char **dependencyNames);
// dtor
void moduleInfoDestroy(ModuleInfo *);

// hashMap between module name and ast node
// specialization of a generic
typedef HashMap ModuleInfoTable;
// ctor
ModuleInfoTable *moduleInfoTableCreate(void);
// get
// returns the node, or NULL if the key is not in the table
ModuleInfo *moduleInfoTableGet(ModuleInfoTable *, char const *key);
// put - note that key is not owned by the table, but the node is
// returns: HT_OK if the insertion was successful
//          HT_EEXISTS if the key exists
int moduleInfoTablePut(ModuleInfoTable *, char const *key, ModuleInfo *data);
// dtor
void moduleInfoTableDestroy(ModuleInfoTable *);

#endif  // TLC_UTIL_MODULEINFOTABLE_H_