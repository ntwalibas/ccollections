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


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "lsearch.h"

/**
 * Performs a linear search on the given collection.
 *
 * @param       collection  the collection to search from.
 * @param       element     the element to search for.
 * @param       compare     the function to use to compare elements.
 *
 * @return      the index of the element searched if found, -1 otherwise.
 */
int lsearch(
    struct Collection const * const collection,
    void const * const needle,
    CComparator compare
) {
    alt_assert(
        collection != NULL,
        "The parameter <collection> cannot be NULL."
    );
    alt_assert(
        collection -> get != NULL,
        "The collection doesn't provide a mechanism for obtain elements "
        "by index/key, and therefore unsearchable."
    );
    alt_assert(
        collection -> atEnd != NULL,
        "The collection doesn't provide a mechanism for checking if "
        "one is at the end of the content it holds, and therefore unsearchable."
    );

    size_t index = 0;
    while (collection -> atEnd(collection, index) == false) {
        void * element = collection -> get(collection, index);
        
        if (compare(element, needle) == 0)
            return index;
        
        index++;
    }

    return -1;
}
