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

static void * _listCollectionGet(struct Collection * const collection, unsigned index);
static void _listCollectionSet(struct Collection * const collection, unsigned index, void * element);
static bool _listCollectionAtEnd(struct Collection const * const collection, unsigned index);

static void * createNode(void * element);
static void deleteNode(struct Node * node, CDeleter deleter);


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
        .get = _listCollectionGet,
        .set = _listCollectionSet,
        .atEnd = _listCollectionAtEnd,
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
        struct Node * next = current -> next;
        deleteNode(current, deleter);
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


/**
 * Pushes an element to the back of the list.
 *
 * @param       list pointer to list to push back onto.
 * @param       element pointer to the element to push back.
 */
void listPushBack(struct List * const list, void * element) {
    char const * message = "The parameter <list> cannot be NULL.";
    if (list == NULL)
        goto exit;

    struct Node * new_tail = createNode(element);
    if (list -> size == 0) {
        list -> head = new_tail;
        list -> tail = new_tail;
        list -> current_node = new_tail;
        list -> current_index = 0;
    }
    else {
        new_tail -> prev = list -> tail;
        list -> tail -> next = new_tail;
        list -> tail = new_tail;
    }

    list -> size++;
    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: listPushBack.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Pushes the element to the front of the list.
 *
 * @param       list pointer to list to push front onto.
 * @param       element pointer to the element to push at the front.
 */
void listPushFront(struct List * const list, void * element) {
    char const * message = "The parameter <list> cannot be NULL.";
    if (list == NULL)
        goto exit;

    struct Node * new_head = createNode(element);
    if (list -> size == 0) {
        list -> head = new_head;
        list -> tail = new_head;
        list -> current_node = new_head;
        list -> current_index = 0;
    }
    else {
        new_head -> next = list -> head;
        list -> head -> prev = new_head;
        list -> head = new_head;
    }

    list -> size++;
    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: listPushFront.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Pops an element at the back of the list.
 *
 * @param       list pointer to list to pop back from.
 *
 * @return      the element at the back of the list.
 */
void * listPopBack(struct List * const list) {
    char const * message = "The parameter <list> cannot be NULL.";
    if (list == NULL)
        goto exit;

    if (list -> size == 0)
        return NULL;
    
    struct Node * old_tail = list -> tail;
    list -> tail = old_tail -> prev;
    list -> tail -> next = NULL;
    if (list -> current_node == old_tail) {
        list -> current_node = list -> head;
        list -> current_index = list -> size - 1;
    }

    list -> size--;

    void * element = old_tail -> element;
    deleteNode(old_tail, NULL);

    return element;

exit:
    fprintf(stderr, "File: %s.\nOperation: listPopBack.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Pops the element at the front of the list.
 *
 * @param       list pointer to list to pop front from.
 *
 * @return      the element at the front of the list.
 */
void * listPopFront(struct List * const list) {
    char const * message = "The parameter <list> cannot be NULL.";
    if (list == NULL)
        goto exit;

    if (list -> size == 0)
        return NULL;
    
    struct Node * old_head = list -> head;
    list -> head = old_head -> next;
    list -> head -> prev = NULL;
    if (list -> current_node == old_head) {
        list -> current_node = list -> head;
        list -> current_index = list -> size - 1;
    }
    
    list -> size--;

    void * element = old_head -> element;
    deleteNode(old_head, NULL);

    return element;

exit:
    fprintf(stderr, "File: %s.\nOperation: listPopFront.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Gets an element at the back of the list.
 *
 * @param       list pointer to list to get the back element from.
 *
 * @return      the element at the back of the list.
 */
void * listBack(struct List const * const list) {
    char const * message = "The parameter <list> cannot be NULL.";
    if (list == NULL)
        goto exit;

    if (list -> size == 0)
        return NULL;
    
    return list -> tail -> element;

exit:
    fprintf(stderr, "File: %s.\nOperation: listBack.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Gets the element at the front of the list.
 *
 * @param       list pointer to list to get the front element from.
 *
 * @return      the element at the front of the list.
 */
void * listFront(struct List const * const list) {
    char const * message = "The parameter <list> cannot be NULL.";
    if (list == NULL)
        goto exit;

    if (list -> size == 0)
        return NULL;
    
    return list -> head -> element;

exit:
    fprintf(stderr, "File: %s.\nOperation: listFront.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Gets the element at the front of the list.
 *
 * @param       list pointer to list to get the front element from.
 *
 * @return      the element at the front of the list.
 */
void * listGet(struct List * const list, unsigned index) {
    return _listCollectionGet(&list -> collection, index);
}

static void * _listCollectionGet(struct Collection * const collection, unsigned index) {
    struct List * const list = (struct List * const) collection;
    char const * message = NULL;
    
    if (list == NULL) {
        message = "The parameter <list> cannot be NULL.";
        goto exit;
    }

    if (index >= list -> size) {
        message = "Index is out of bounds.";
        goto exit;
    }

    if (list -> size == 0)
        return NULL;
    
    bool move_right = (index > list -> current_index) ? true : false;
    while (list -> current_index != index) {
        list -> current_node = move_right ? list -> current_node -> next : list -> current_node -> prev;
        list -> current_index = move_right ? list -> current_index + 1 : list -> current_index - 1;
    }

    return list -> current_node -> element;

exit:
    fprintf(stderr, "File: %s.\nOperation: _listCollectionGet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Sets the element at the given position in the list.
 *
 * @param       list pointer to list to get the element from.
 */
void listSet(struct List * const list, unsigned index, void * element) {
    _listCollectionSet(&list -> collection, index, element);
}

static void _listCollectionSet(struct Collection * const collection, unsigned index, void * element) {
    struct List * const list = (struct List * const) collection;
    char const * message = NULL;
    
    if (list == NULL) {
        message = "The parameter <list> cannot be NULL.";
        goto exit;
    }

    if (index >= list -> size) {
        message = "Index is out of bounds.";
        goto exit;
    }
    
    bool move_right = (index > list -> current_index) ? true : false;
    while (list -> current_index != index) {
        list -> current_node = move_right ? list -> current_node -> next : list -> current_node -> prev;
        list -> current_index = move_right ? list -> current_index + 1 : list -> current_index - 1;
    }

    list -> current_node -> element = element;
    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: _listCollectionSet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Inserts the element at the given position in the list, moving the other elements to the right.
 *
 * @param       list    pointer to list to get the element from.
 * @param       index   the index where to insert the element.
 * @param       element the element to insert.
 */
void listInsert(struct List * const list, unsigned index, void * element) {
    char const * message = NULL;
    
    if (list == NULL) {
        message = "The parameter <list> cannot be NULL.";
        goto exit;
    }

    if (index >= list -> size) {
        message = "Index is out of bounds.";
        goto exit;
    }
    
    bool move_right = (index > list -> current_index) ? true : false;
    while (list -> current_index != index) {
        list -> current_node = move_right ? list -> current_node -> next : list -> current_node -> prev;
        list -> current_index = move_right ? list -> current_index + 1 : list -> current_index - 1;
    }

    struct Node * new_node = createNode(element);
    new_node -> prev = list -> current_node -> prev;
    new_node -> next = list -> current_node;
    list -> current_node -> prev = new_node;
    list -> current_node = new_node;

    if (list -> current_index == 0)
        list -> head = new_node;

    if (list -> current_index == list -> size - 1)
        list -> tail = new_node;

    list -> size++;
    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: listInsert.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Resets the current node and index to the beginning of the list.
 *
 * @param       list pointer to list to get the element from.
 */
void listResetCurrent(struct List * const list) {
    char const * message = "The parameter <list> cannot be NULL.";
    if (list == NULL)
        goto exit;

    list -> current_node = list -> head;
    list -> current_index = 0;

    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: listResetCurrent.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


static bool _listCollectionAtEnd(struct Collection const * const collection, unsigned index) {
    struct List const * const list = (struct List const * const) collection;
    char const * message = NULL;
    
    if (list == NULL) {
        message = "The parameter <list> cannot be NULL.";
        goto exit;
    }

    if (list -> size == 0) {
        message = "The list is empty, cannot check if at end.";
        goto exit;
    }
    
    return index >= list -> size - 1;

exit:
    fprintf(stderr, "File: %s.\nOperation: _listCollectionAtEnd.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


static void * createNode(void * element) {
    struct Node * node = malloc(sizeof *node);
    if (node == NULL)
        return NULL;
    
    node -> prev = NULL;
    node -> next = NULL;
    node -> element = element;

    return node;
}

static void deleteNode(struct Node * node, CDeleter deleter) {
    if (deleter != NULL)
        deleter(& node -> element);
    
    free(node);
}