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


 /* This choice is justified by the following:
  * - It is easy to grow the deque from easier end easily if needed.
  * - It is easy to shrink the deque easily so as not to unnecessarily consume available memory.
  *
  * It could be argued that the memory allocations and deallocations are expensive,
  * but that can be managed per application basis.
  * What's expensive (e.g. in a ring buffer implementation) is allocating memory,
  * then growing painfully and then shrink in painfully.
  *
  * So this solution seems like a good trade-off between:
  * 1. Code clarity and implementation,
  * 2. And efficiency of resource utilization.
  *
  * /!\
  * Note that in the functions below operating on the buffer we don't have many checks.
  * And that's because this code is internal, and we trust ourselves to not pass NULL pointers around.
  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

float deque_growth_factor = 2;


struct Buffer {
    void ** contents;
    unsigned capacity;
    unsigned size;
};

static void * newBuffer(unsigned capacity) {
    struct Buffer * buffer = malloc(sizeof *buffer);

    if (buffer == NULL)
        return NULL;

    void ** contents = malloc(capacity * sizeof(void *));
    if (contents == NULL) {
        free(buffer);
        return NULL;
    }

    buffer -> contents = contents;
    buffer -> capacity = capacity;
    buffer -> size = 0;
    
    return buffer;
}

static void deleteBuffer(struct Buffer * buffer) {
    if (buffer == NULL)
        return;
    
    free(buffer -> contents);
    free(buffer);
}

static void * resizeBuffer(struct Buffer * buffer) {
    unsigned new_capacity = deque_growth_factor * buffer -> capacity;
    void **contents = realloc(buffer -> contents, new_capacity * sizeof(void *));

    // If we could not resize the buffer, we return NULL
    if (contents == NULL)
        return NULL;
        
    buffer -> contents = contents;
    buffer -> capacity = new_capacity;
    return buffer;
}

static bool bufferPushBack(struct Buffer * const buffer, void * content) {
    if (buffer -> size == buffer -> capacity) {
        if (resizeBuffer(buffer) == NULL)
            return false;
    }

    buffer -> contents[buffer -> size++] = content;

    return true;
}

static bool bufferPushFront(struct Buffer * const buffer, void * content) {
    if (buffer -> size == buffer -> capacity) {
        if (resizeBuffer(buffer) == NULL)
            return false;
    }

    if (buffer -> size > 0)
        memmove(buffer -> contents + 1, buffer -> contents, buffer -> size * sizeof(void *));

    buffer -> contents[0] = content;
    buffer -> size++;

    return true;
}

static void * bufferPopBack(struct Buffer * const buffer) {
    return buffer -> contents[-- buffer -> size];
}

static void * bufferPopFront(struct Buffer * const buffer) {
    void * content = buffer -> contents[0];
    memmove(buffer -> contents, buffer -> contents + 1, (buffer -> size - 1) * sizeof(void *));
    return content;
}

static inline void * bufferGet(struct Buffer const * const buffer, unsigned index) {
    return buffer -> contents[index];
}
