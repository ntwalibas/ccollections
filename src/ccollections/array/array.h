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

#ifndef CCOLLECTIONS_ARRAY_H
#define CCOLLECTIONS_ARRAY_H

#include <stdbool.h>
#include <stddef.h>


#include "../interfaces/interfaces.h"

extern float array_growth_factor;

struct Array {
    struct Collection collection;
    void ** elements;
    unsigned capacity;
    unsigned size;
};


/**
 * Initializes the array
 *
 * @return      the newly created array.
 */
struct Array * newArray(unsigned initial_capacity);


/**
 * Frees the memory occupied by the array.
 *
 * @param       array pointer to memory occupied by the array.
 */
void deleteArray(struct Array ** const array);


/**
 * Resizes the given array to higher capacity.
 *
 * @param       array pointer to array to resize.
 * @param       new_capacity the new capacity of the array.
 *
 * @return      the newly resized array.
 */
struct Array * resizeArray(struct Array * const array, unsigned new_capacity);


/**
 * Check if the array is empty.
 *
 * @param       array pointer to the array which content to check.
 *
 * @return      true if the array is empty, false otherwise.
 */
bool isArrayEmpty(struct Array const * const array);


/**
 * Appends an element at the end of the array.
 *
 * @param       array pointer to array to append an element to.
 * @param       element pointer to the element to append to the array.
 */
void arrayPushBack(struct Array * const array, void * element);


/**
 * Get the element at the specified index.
 *
 * @param       array pointer to array to use.
 * @param       index the index at which to look.
 *
 * @return      the element at the specified index.
 */
void * arrayGet(struct Array const * const array, unsigned index);


/**
 * Set an element at the specified index.
 *
 * @param       array pointer to array to use.
 * @param       index the index at which to write.
 * @param       element the element to write at the specified index.
 */
void arraySet(struct Array * const array, unsigned index, void * element);

#endif
