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


#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"

static void deleteNode(struct Node * node);


/**
 * Initializes the list
 *
 * @return      the newly created list.
 */
struct List * newList() {
    struct List * list = malloc(sizeof *list);
    if (list == NULL)
        return NULL;

    struct Collection collection = {
        .get = NULL,
        .set = NULL,
        .atEnd = NULL,
    };

    list -> collection = collection;
    list -> head = NULL;
    list -> tail = NULL;
    list -> size = 0;
    list -> current_node = NULL;
    list -> current_index = 0;

    return list;
}


/**
 * Frees the memory occupied by the list.
 *
 * @param       list pointer to memory occupied by the list.
 */
void deleteList(struct List ** const list, CDeleter deleter) {
    if (list == NULL)
        return;

    if (* list == NULL)
        return;

    struct Node * current = (* list) -> head;
    while (current != NULL) {
        if (deleter != NULL)
            deleter(& current -> element);
        
        struct Node * next = current -> next;
        deleteNode(current);
        current = next;
    }

    free(* list);
    * list = NULL;
}


/**
 * Check if the list is empty.
 *
 * @param       list pointer to the list which content to check.
 *
 * @return      true if the list is empty, false otherwise.
 */
bool isListEmpty(struct List const * const list) {
    char const * message = "The parameter <list> cannot be NULL.";
    if (list == NULL)
        goto exit;

    return list -> size == 0;

exit:
    fprintf(stderr, "File: %s.\nOperation: isListEmpty.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


static void deleteNode(struct Node * node) {

}
