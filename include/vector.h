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

#ifndef CCOLLECTIONS_VECTOR_H
#define CCOLLECTIONS_VECTOR_H

#include <stdbool.h>
#include <stddef.h>


#include "common.h"

extern float vector_growth_factor;

struct Vector {
    struct Collection collection;
    void ** elements;
    unsigned capacity;
    unsigned size;
};


/**
 * Initializes the vector
 *
 * @return      the newly created vector.
 */
struct Vector * newVector(unsigned initial_capacity);


/**
 * Frees the memory occupied by the vector.
 *
 * @param       vector pointer to memory occupied by the vector.
 */
void deleteVector(struct Vector ** const vector, CDeleter deleter);


/**
 * Resizes the given vector to higher capacity.
 *
 * @param       vector pointer to vector to resize.
 * @param       new_capacity the new capacity of the vector.
 *
 * @return      the newly resized vector.
 */
struct Vector * resizeVector(struct Vector * const vector, unsigned new_capacity);


/**
 * Check if the vector is empty.
 *
 * @param       vector pointer to the vector which content to check.
 *
 * @return      true if the vector is empty, false otherwise.
 */
bool isVectorEmpty(struct Vector const * const vector);


/**
 * Appends an element at the end of the vector.
 *
 * @param       vector pointer to vector to append an element to.
 * @param       element pointer to the element to append to the vector.
 *
 * @return      true if the element was added, false otherwise (probably due to insufficient memory)
 */
bool vectorPushBack(struct Vector * const vector, void * element);


/**
 * Get the element at the specified index.
 *
 * @param       vector pointer to vector to use.
 * @param       index the index at which to look.
 *
 * @return      the element at the specified index.
 */
void * vectorGet(struct Vector * const vector, unsigned index);


/**
 * Set an element at the specified index.
 *
 * @param       vector pointer to vector to use.
 * @param       index the index at which to write.
 * @param       element the element to write at the specified index.
 */
void vectorSet(struct Vector * const vector, unsigned index, void * element);

#endif
