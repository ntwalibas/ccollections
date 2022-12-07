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

#include "vector.h"

static void * _vectorCollectionGet(struct Collection const * const collection, unsigned index);
static void _vectorCollectionSet(struct Collection * const collection, unsigned index, void * element);
static bool _vectorCollectionAtEnd(struct Collection const * const collection, unsigned index);

float vector_growth_factor = 1.75;


/**
 * Initializes the vector
 *
 * @return      the newly created vector.
 */
struct Vector * newVector(unsigned initial_capacity) {
    const char * message = "Initial vector capacity cannot be zero.";
    if(initial_capacity == 0)
        goto exit;

    struct Vector * vector = malloc(sizeof *vector);
    if (vector == NULL)
       return NULL;

    vector -> elements = malloc(initial_capacity * sizeof *vector -> elements);
    if (vector -> elements == NULL) {
        free(vector);
        return NULL;
    }

    struct Collection collection = {
        .get = _vectorCollectionGet,
        .set = _vectorCollectionSet,
        .atEnd = _vectorCollectionAtEnd,
    };

    vector -> collection = collection;
    vector -> capacity = initial_capacity;
    vector -> size = 0;

    return vector;

exit:
    fprintf(stderr, "File: %s.\nOperation: newVector.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Frees the memory occupied by the vector.
 *
 * @param       vector pointer to memory occupied by the vector.
 */
void deleteVector(struct Vector ** const vector, CDeleter deleter) {
    if (vector == NULL)
        return;

    if (* vector == NULL)
        return;
    
    if (deleter != NULL) {
        for(unsigned i = 0; i < (* vector) -> size; i++) {
            void * element = (* vector) -> elements[i];
            deleter(& element);
        }
    }

    free((* vector) -> elements);
    free(* vector);
    * vector = NULL;
}


/**
 * Resizes the given vector to higher capacity.
 *
 * @param       vector pointer to vector to resize.
 * @param       new_capacity the new capacity of the vector.
 *
 * @return      the newly resized vector.
 */
struct Vector * resizeVector(struct Vector * const vector, unsigned new_capacity) {
    const char * message = NULL;
    
    if (vector == NULL) {
        message = "The parameter <vector> cannot be NULL.";
        goto exit;
    }

    if (new_capacity <= vector -> capacity) {
        message = "The new capacity cannot less or equal to the existing capacity.";
        goto exit;
    }

    void ** new_elements = realloc(vector -> elements, new_capacity * sizeof *vector -> elements);
    if (new_elements == NULL)
        return NULL;

    vector -> elements = new_elements;
    vector -> capacity = new_capacity;

    return vector;

exit:
    fprintf(stderr, "File: %s.\nOperation: resizeVector.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Check if the vector is empty.
 *
 * @param       vector pointer to the vector which content to check.
 *
 * @return      true if the vector is empty, false otherwise.
 */
bool isVectorEmpty(struct Vector const * const vector) {
    char const * message = "The parameter <vector> cannot be NULL.";
    if (vector == NULL)
        goto exit;

    return vector -> size == 0;

exit:
    fprintf(stderr, "File: %s.\nOperation: isVectorEmpty.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Appends an element at the end of the vector.
 *
 * @param       vector pointer to vector to append an element to.
 * @param       element pointer to the element to append to the vector.
 */
void vectorPushBack(struct Vector * const vector, void * element) {
    char const * message = NULL;
    
    if (vector == NULL) {
        message = "The parameter <vector> cannot be NULL.";
        goto exit;
    }

    if (vector -> size == vector -> capacity) {
        unsigned new_capacity = vector_growth_factor * vector -> capacity;
        void ** new_elements = realloc(vector -> elements, new_capacity * sizeof *vector -> elements);
        if (new_elements == NULL) {
            message = "Failed to allocate space for new elements.";
            goto exit;
        }

        vector -> elements = new_elements;
        vector -> capacity = new_capacity;
    }

    vector -> elements[vector -> size++] = element;

    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: vectorPushBack.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Get the element at the specified index.
 *
 * @param       vector pointer to vector to use.
 * @param       index the index at which to look.
 *
 * @return      the element at the specified index.
 */
void * vectorGet(struct Vector const * const vector, unsigned index) {
    return _vectorCollectionGet(&vector -> collection, index);
}

static void * _vectorCollectionGet(struct Collection const * const collection, unsigned index) {
    struct Vector const * const vector = (struct Vector const * const) collection;
    char const * message = NULL;
    
    if (vector == NULL) {
        message = "The parameter <vector> cannot be NULL.";
        goto exit;
    }

    if (vector -> size == 0) {
        message = "The vector is empty, cannot get elements.";
        goto exit;
    }

    if (index >= vector -> size) {
        message = "The index is out of bounds.";
        goto exit;
    }

    return vector -> elements[index];

exit:
    fprintf(stderr, "File: %s.\nOperation: _vectorCollectionGet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Set an element at the specified index.
 *
 * @param       vector pointer to vector to use.
 * @param       index the index at which to write.
 * @param       element the element to write at the specified index.
 */
void vectorSet(struct Vector * const vector, unsigned index, void * element) {
    return _vectorCollectionSet(&vector -> collection, index, element);
}

static void _vectorCollectionSet(struct Collection * const collection, unsigned index, void * element) {
    struct Vector * const vector = (struct Vector * const) collection;
    char const * message = NULL;

    if (vector == NULL) {
        message = "The parameter <vector> cannot be NULL.";
        goto exit;
    }

    if (vector -> size == 0) {
        message = "The vector is empty, cannot set elements.";
        goto exit;
    }

    if (index >= vector -> size) {
        message = "The index is out of bounds.";
        goto exit;
    }

    vector -> elements[index] = element;
    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: _vectorCollectionSet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


static bool _vectorCollectionAtEnd(struct Collection const * const collection, unsigned index) {
    struct Vector const * const vector = (struct Vector const * const) collection;
    const char * message = NULL;

    if (vector == NULL) {
        message = "The parameter <vector> cannot be NULL.";
        goto exit;
    }

    if (vector -> size == 0) {
        message = "The vector is empty, cannot check if at end.";
        goto exit;
    }

    return index >= vector -> size;

exit:
    fprintf(stderr, "File: %s.\nOperation: _vectorCollectionAtEnd.\nMessage: %s\n", __FILE__, message);
    exit(74);
}

