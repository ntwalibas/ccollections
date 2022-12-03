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


#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "array.h"

static void * _arrayCollectionGet(struct Collection const * const collection, size_t index);
static void _arrayCollectionSet(struct Collection * const collection, size_t index, void * element);
static int _arrayCollectionCompare(struct Collection const * const collection, size_t index_1, size_t index_2);

float array_growth_factor = 1.75;


/**
 * Initializes the array
 *
 * @return      the newly created array.
 */
struct Array * newArray(size_t initial_capacity) {
    const char * message = "Initial array capacity cannot be zero.";
    if(initial_capacity == 0)
        goto exit;

    struct Array * array = malloc(sizeof *array);
    if (array == NULL)
       return NULL;

    array -> elements = malloc(initial_capacity * sizeof *array -> elements);
    if (array -> elements == NULL) {
        free(array);
        return NULL;
    }

    struct Indexable * indexable_vtable = malloc(sizeof *indexable_vtable);
    indexable_vtable -> get = _arrayCollectionGet;
    indexable_vtable -> set = _arrayCollectionSet;

    struct Comparable * comparable_vtable = malloc(sizeof *comparable_vtable);
    comparable_vtable -> compare = _arrayCollectionCompare;

    struct Collection super = {
        .indexable_vtable = indexable_vtable,
        .comparable_vtable = comparable_vtable
    };

    array -> super = super;
    array -> capacity = initial_capacity;
    array -> size = 0;

    return array;

exit:
    fprintf(stderr, "File: %s.\nOperation: newArray.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Frees the memory occupied by the array.
 *
 * @param       array pointer to memory occupied by the array.
 */
void deleteArray(struct Array ** const array) {
    if (array == NULL)
        return;

    if (* array == NULL)
        return;

    free((void *)(* array) -> super.indexable_vtable);
    free((void *)(* array) -> super.comparable_vtable);
    free((* array) -> elements);
    free(* array);
    * array = NULL;
}


/**
 * Resizes the given array to higher size.
 *
 * @param       array pointer to array to resize.
 * @param       new_capacity the new capacity of the array.
 *
 * @return      the newly created array.
 */
struct Array * resizeArray(struct Array * const array, size_t new_capacity) {
    const char * message = NULL;
    
    if (array == NULL) {
        message = "The parameter <array> cannot be NULL.";
        goto exit;
    }

    if (new_capacity <= array -> capacity) {
        message = "The new capacity cannot less or equal to the existing capacity.";
        goto exit;
    }

    void ** new_elements = realloc(array -> elements, new_capacity * sizeof *array -> elements);
    if (new_elements == NULL)
        return NULL;

    array -> elements = new_elements;
    array -> capacity = new_capacity;

    return array;

exit:
    fprintf(stderr, "File: %s.\nOperation: resizeArray.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Check if the array is empty.
 *
 * @param       array pointer to the array which content to check.
 *
 * @return      true if the array is empty, false otherwise.
 */
bool isArrayEmpty(struct Array const * const array) {
    char const * message = "The parameter <array> cannot be NULL.";
    if (array == NULL)
        goto exit;

    return array -> size == 0;

exit:
    fprintf(stderr, "File: %s.\nOperation: isArrayEmpty.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Appends an element at the end of the array.
 *
 * @param       array pointer to array to append an element to.
 * @param       element pointer to the element to append to the array.
 */
void arrayAppend(struct Array * const array, void * element) {
    char const * message = NULL;
    
    if (array == NULL) {
        message = "The parameter <array> cannot be NULL.";
        goto exit;
    }

    if (array -> size == array -> capacity) {
        size_t new_capacity = array_growth_factor * array -> capacity;
        void ** new_elements = realloc(array -> elements, new_capacity * sizeof *array -> elements);
        if (new_elements == NULL) {
            message = "Failed to allocate space for new elements.";
            goto exit;
        }

        array -> elements = new_elements;
        array -> capacity = new_capacity;
    }

    array -> elements[array -> size++] = element;

    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: arrayAppend.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Get the element at the specified index.
 *
 * @param       array pointer to array to use.
 * @param       index the index at which to look.
 *
 * @return      the element at the specified index.
 */
void * arrayGet(struct Array const * const array, size_t index) {
    return _arrayCollectionGet((struct Collection const * const) array, index);
}

static void * _arrayCollectionGet(struct Collection const * const collection, size_t index) {
    struct Array const * const array = (struct Array const * const) collection;
    char const * message = NULL;
    
    if (array == NULL) {
        message = "The parameter <array> cannot be NULL.";
        goto exit;
    }

    if (array -> size == 0) {
        message = "The array is empty, cannot get elements.";
        goto exit;
    }

    if (index >= array -> size) {
        message = "The index is out of bounds.";
        goto exit;
    }

    return array -> elements[index];

exit:
    fprintf(stderr, "File: %s.\nOperation: _arrayCollectionGet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Set an element at the specified index.
 *
 * @param       array pointer to array to use.
 * @param       index the index at which to write.
 * @param       element the element to write at the specified index.
 */
void arraySet(struct Array * const array, size_t index, void * element) {
    return _arrayCollectionSet((struct Collection * const) array, index, element);
}

static void _arrayCollectionSet(struct Collection * const collection, size_t index, void * element) {
    struct Array * const array = (struct Array * const) collection;
    char const * message = NULL;

    if (array == NULL) {
        message = "The parameter <array> cannot be NULL.";
        goto exit;
    }

    if (array -> size == 0) {
        message = "The array is empty, cannot set elements.";
        goto exit;
    }

    if (index >= array -> size) {
        message = "The index is out of bounds.";
        goto exit;
    }

    array -> elements[index] = element;
    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: _arrayCollectionSet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


static int _arrayCollectionCompare(struct Collection const * const collection, size_t index_1, size_t index_2) {
    struct Array const * const array = (struct Array const * const) collection;
    const char * message = NULL;

    if (array == NULL) {
        message = "The parameter <array> cannot be NULL.";
        goto exit;
    }

    if (array -> size == 0) {
        message = "The array is empty, cannot compare indexes.";
        goto exit;
    }

    if (index_1 >= array -> size) {
        message = "Index_1 is out of bounds.";
        goto exit;
    }

    if (index_2 >= array -> size) {
        message = "Index_2 is out of bounds.";
        goto exit;
    }

    return (index_1 > index_2) - (index_1 < index_2);

exit:
    fprintf(stderr, "File: %s.\nOperation: _arrayCollectionCompare.\nMessage: %s\n", __FILE__, message);
    exit(74);
}
