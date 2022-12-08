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

#ifndef CCOLLECTIONS_LIST_H
#define CCOLLECTIONS_LIST_H

#include <stdbool.h>
#include <stddef.h>

#include "../interfaces/interfaces.h"


struct Node {
    struct Node * prev;
    struct Node * next;
    void * element;
};

struct List {
    struct Collection collection;
    struct Node * head;
    struct Node * tail;
    unsigned size;
    struct {
        struct Node * current_node;
        unsigned current_index;
    };
};


/**
 * Initializes the list
 *
 * @return      the newly created list.
 */
struct List * newList();


/**
 * Frees the memory occupied by the list.
 *
 * @param       list pointer to memory occupied by the list.
 */
void deleteList(struct List ** const list, CDeleter deleter);


/**
 * Check if the list is empty.
 *
 * @param       list pointer to the list which content to check.
 *
 * @return      true if the list is empty, false otherwise.
 */
bool isListEmpty(struct List const * const list);

#endif
