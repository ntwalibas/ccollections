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

#ifndef CCOLLECTIONS_ARRAY_H
#define CCOLLECTIONS_ARRAY_H

#include <stdbool.h>
#include <stddef.h>

extern float array_growth_factor;

struct Array {
    void ** elements;
    size_t size;
    size_t count;
};


/**
 * Initializes the array
 *
 * @return      the newly created array.
 */
struct Array * newArray(size_t size);


/**
 * Frees the memory occupied by the array.
 *
 * @param       array pointer to memory occupied by the array.
 */
void deleteArray(struct Array ** const array);


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
void arrayAppend(struct Array * const array, void * element);

#endif
