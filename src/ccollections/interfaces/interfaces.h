/*  This file is part of the CCollections library
 * 
 *  Copyright (c) 2022- Ntwali B. Toussaint
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef CCOLLECTIONS_INTERFACE_H
#define CCOLLECTIONS_INTERFACE_H

#include <stdbool.h>
#include <stddef.h>

struct Collection;

struct Indexable {
    void * (* get)(struct Collection const * const collection, size_t index);
    void (* set)(struct Collection * const collection, size_t index, void * element);
};


struct Comparable {
    int (* compare)(struct Collection const * const collection, size_t index_1, size_t index_2);
};


struct Collection {
    struct Indexable const * indexable_vtable;
    struct Comparable const * comparable_vtable;
};

#endif