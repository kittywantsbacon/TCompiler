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

// translation phase

#ifndef TLC_TRANSLATE_TRANSLATE_H_
#define TLC_TRANSLATE_TRANSLATE_H_

#include "util/container/hashMap.h"
#include "util/container/vector.h"

struct ModuleAstMapPair;
struct Frame;
struct Access;
struct LabelGenerator;
typedef struct Frame *(*FrameCtor)(void);
typedef struct Access *(*GlobalAccessCtor)(char *label);
typedef struct LabelGenerator *(*LabelGeneratorCtor)(void);
typedef Vector FragmentVector;

typedef HashMap FileFragmentVectorMap;
void fileFragmentVectorMapInit(FileFragmentVectorMap *);
FragmentVector *fileFragmentVectorMapGet(FileFragmentVectorMap *,
                                         char const *file);
int fileFragmentVectorMapPut(FileFragmentVectorMap *, char *file,
                             FragmentVector *vector);
void fileFragmentVectorMapUninit(FileFragmentVectorMap *);

void translate(FileFragmentVectorMap *, struct ModuleAstMapPair *, FrameCtor,
               GlobalAccessCtor, LabelGeneratorCtor);

#endif  // TLC_TRANSLATE_TRANSLATE_H_