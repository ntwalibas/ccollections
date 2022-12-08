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


/**
 * Pushes an element to the back of the list.
 *
 * @param       list pointer to list to push back onto.
 * @param       element pointer to the element to push back.
 */
void listPushBack(struct List * const list, void * element);


/**
 * Pushes the element to the front of the list.
 *
 * @param       list pointer to list to push front onto.
 * @param       element pointer to the element to push at the front.
 */
void listPushFront(struct List * const list, void * element);


/**
 * Pops an element at the back of the list.
 *
 * @param       list pointer to list to pop back from.
 *
 * @return      the element at the back of the list.
 */
void * listPopBack(struct List * const list);


/**
 * Pops the element at the front of the list.
 *
 * @param       list pointer to list to pop front from.
 *
 * @return      the element at the front of the list.
 */
void * listPopFront(struct List * const list);


/**
 * Gets an element at the back of the list.
 *
 * @param       list pointer to list to get the back element from.
 *
 * @return      the element at the back of the list.
 */
void * listBack(struct List const * const list);


/**
 * Gets the element at the front of the list.
 *
 * @param       list pointer to list to get the front element from.
 *
 * @return      the element at the front of the list.
 */
void * listFront(struct List const * const list);


/**
 * Gets the element at the front of the list.
 *
 * @param       list pointer to list to get the front element from.
 *
 * @return      the element at the front of the list.
 */
void * listGet(struct List * const list, unsigned index);


/**
 * Sets the element at the given position in the list, replacing the existing element.
 *
 * @param       list pointer to list to get the element from.
 */
void listSet(struct List * const list, unsigned index, void * element);


/**
 * Inserts the element at the given position in the list, moving the other elements to the right.
 *
 * @param       list    pointer to list to get the element from.
 * @param       index   the index where to insert the element.
 * @param       element the element to insert.
 */
void listInsert(struct List * const list, unsigned index, void * element);


/**
 * Resets the current node and index to the beginning of the list.
 *
 * @param       list pointer to list to get the element from.
 */
void listResetCurrent(struct List * const list);

#endif
