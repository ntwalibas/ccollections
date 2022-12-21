/*  This file is part of the CCollections library.
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

#ifndef CCOLLECTIONS_LSEARCH_H
#define CCOLLECTIONS_LSEARCH_H

#include <stddef.h>

#include "common.h"

/**
 * Performs a linear search on the given collection.
 *
 * @param       collection  the collection to search from.
 * @param       element     the element to search for.
 * @param       compare     the function to use to compare elements.
 *
 * @return      the index of the element searched if found, -1 otherwise.
 */
int lsearch(struct Collection const * const collection, void const * const element, CComparator compare);

#endif
