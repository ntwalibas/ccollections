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

float array_growth_factor = 1.75;


/**
 * Initializes the array
 *
 * @return      the newly created array.
 */
struct Array * newArray(size_t initial_size) {
    const char * message = "Initial array size cannot be zero.";
    if(initial_size == 0)
        goto exit;

    struct Array * array = malloc(sizeof *array);
    if (array == NULL)
       return NULL;

    array -> elements = malloc(initial_size * sizeof *array -> elements);
    if (array -> elements == NULL) {
        free(array);
        return NULL;
    }

    array -> size = initial_size;
    array -> count = 0;

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

    free((* array) -> elements);
    free(* array);
    * array = NULL;
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

    return array -> count == 0;

exit:
    fprintf(stderr, "File: %s.\nOperation: isArrayEmpty.\nMessage: %s\n", __FILE__, message);
    exit(74);
}
