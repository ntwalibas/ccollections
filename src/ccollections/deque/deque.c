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
#include <string.h>
#include <stdio.h>

#include "deque.h"

/* 
 * This implementation uses a dynamic array with fixed-sized arrays.
 * It is heavily based off Martin Broadhrust's implementation at:
 * (https://web.archive.org/web/20220318120354/http://www.martinbroadhurst.com/deque.html)
 *
 * The above choice is justified by the following:
 * - It is easy to grow the deque easily if needed.
 * - It is easy to shrink the deque easily so as not to unnecessarily consume available memory.
 * - If the deque is used a queue, no memory is left unused (one half could be)
 *
 * It could be argued that the memory allocations and deallocations are expensive,
 * but that can be managed per application basis by allocating as much capacity as needed upfront.
 *
 * So this solution seems like a good trade-off between:
 * 1. Code clarity and implementation,
 * 2. Efficiency of resource utilization.
 */


static void * newBuffer(unsigned capacity);
static void deleteBuffer(struct Buffer * buffer);
static bool bufferPushBack(struct Buffer * const buffer, void * content);
static bool bufferPushFront(struct Buffer * const buffer, void * content);
static void * bufferPopBack(struct Buffer * const buffer);
static void * bufferPopFront(struct Buffer * const buffer);
static inline void * bufferGet(struct Buffer const * const buffer, unsigned index);

float deque_growth_factor = 2;


/**
 * Initializes the deque
 *
 * @return      the newly created deque.
 */
struct Deque * newDeque(unsigned capacity) {
    const char * message = NULL;

    struct Collection collection = {
        .get = NULL,
        .set = NULL,
        .atEnd = NULL,
    };

    if (capacity == 0) {
        message = "Deque capacity cannot be zero.";
        goto exit;
    }

    struct Deque * deque = malloc(sizeof *deque);
    if (deque == NULL)
        return NULL;

    // Set the main buffer for this
    struct Buffer * buffer = newBuffer(1);
    if (buffer == NULL) {
        free(deque);
        return NULL;
    }

    deque -> collection = collection;
    deque -> buffer = buffer;
    deque -> front_empty = true;
    deque -> back_empty = true;
    deque -> front = 0;
    deque -> back = 0;
    deque -> capacity = capacity;
    deque -> size = 0;

    return deque;

exit:
    fprintf(stderr, "File: %s.\nOperation: newDeque.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Frees the memory occupied by the deque.
 *
 * @param       deque pointer to memory occupied by the deque.
 */
void deleteDeque(struct Deque ** const deque) {
    if (deque == NULL)
        return;
    
    if (* deque == NULL)
        return;

    deleteBuffer((* deque) -> buffer);
    free(* deque);
    * deque = NULL;
}


/**
 * Check if the deque is empty.
 *
 * @param       deque pointer to the deque which content to check.
 *
 * @return      true if the deque is empty, false otherwise.
 */
bool isDequeEmpty(struct Deque const * const deque) {
    const char * message = "The parameter <deque> cannot be NULL.";
    if (deque == NULL)
        goto exit;

    return deque -> size == 0;

exit:
    fprintf(stderr, "File: %s.\nOperation: isDequeEmpty.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Check if the deque is full.
 *
 * @param       deque pointer to the deque which content to check.
 *
 * @return      true if the deque is full, false otherwise.
 */
bool isDequeFull(struct Deque const * const deque) {
    const char * message = "The parameter <deque> cannot be NULL.";
    if (deque == NULL)
        goto exit;

    if (deque -> size == 0)
        return false;
    
    if (deque -> front == 0 && deque -> back == deque -> capacity - 1)
        return true;
    
    return false;

exit:
    fprintf(stderr, "File: %s.\nOperation: isDequeFull.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Pushes an element to the back of the deque.
 *
 * @param       deque pointer to deque to push back onto.
 * @param       element pointer to the element to push back.
 */
void dequePushBack(struct Deque * const deque, void * element) {
    char const * message = NULL;
    unsigned index = 0;
    
    if (deque == NULL) {
        message = "The parameter <deque> cannot be NULL.";
        goto exit;
    }

    // If we don't have space for back elements, we request it
    // Same if we ran out of space for back elements
    if (deque -> back_empty || deque -> back == deque -> capacity - 1) {
        bool back_buffer_ready = bufferPushBack(deque -> buffer, malloc(deque -> capacity * sizeof(void *)));
        if (! back_buffer_ready) {
            message = "Failed to allocate space for new elements to push to the back.";
            goto exit;
        }
    }

    // If we haven't pushed anything on the back buffer, we start now
    if (deque -> back_empty)
        index = 0;

    // If we are the end of the current back buffer, we start anew on the next one
    else if (deque -> back == deque  -> capacity - 1)
        index = 0;

    // Otherwise, we are in the middle of the current back buffer, we advance
    else
        index = deque -> back + 1;

    // Add the element to the deque and update our position in the back
    ((void **) bufferGet(deque -> buffer, (deque -> buffer) -> size - 1))[index] = element;
    deque -> back = index;
    deque -> back_empty = false;
    deque -> size += 1;

    // If we have only one element on the deque, we make sure that we can pop it from any direction
    if (deque -> size == 1)
        deque -> front = deque -> back;
    
    // TODO: explain the code below
    if (deque -> buffer -> size == 1)
        deque -> front_empty = false;

    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: dequePushBack.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Pushes an element to the front of the deque.
 *
 * @param       deque pointer to deque to push front onto.
 * @param       element pointer to the element to push at the front.
 */
void dequePushFront(struct Deque * const deque, void * element) {
    char const * message = NULL;
    if (deque == NULL) {
        message = "The parameter <deque> cannot be NULL.";
        goto exit;
    }

    unsigned index = deque -> capacity - 1;

    // If we don't have space for front elements, we request it
    // Same if we ran out of space for front elements
    if (deque -> front_empty || deque -> front == 0) {
        bool front_buffer_ready = bufferPushFront(deque -> buffer, malloc(deque -> capacity * sizeof(void *)));
        if (! front_buffer_ready) {
            message = "Failed to allocate space for new elements to push to the front.";
            goto exit;
        }
    }

    // If we haven't pushed anything on the front buffer, we start now
    if (deque -> front_empty)
        index = deque -> capacity - 1;

    // If we are the end of the current front buffer, we start anew on the next one
    else if (deque -> front == 0)
        index = deque -> capacity - 1;

    // Otherwise, we are in the middle of the current front buffer, we move backwards
    else
        index = deque -> front - 1;

    // Add the element to the deque and update our position in the front
    ((void **) bufferGet(deque -> buffer, 0))[index] = element;
    deque -> front = index;
    deque -> front_empty = false;
    deque -> size += 1;

    // If we have only one element on the deque, we make sure that we can pop it from any direction
    if (deque -> size == 1)
        deque -> back = deque -> front;

    // TODO: explain the code below
    if (deque -> buffer -> size == 1)
        deque -> back_empty = false;
    
    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: dequePushFront.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Pops an element at the back of the deque.
 *
 * @param       deque pointer to deque to pop back from.
 *
 * @return      the element at the back of the deque.
 */
void * dequePopBack(struct Deque * const deque) {
    char const * message = NULL;

    if (deque == NULL) {
        message = "The parameter <deque> cannot be NULL.";
        goto exit;
    }
    
    if (deque -> back_empty && deque -> buffer -> size > 1) {
        free(bufferPopBack(deque -> buffer));
        deque -> back_empty = false;
    }

    if (deque -> size == 0)
        return NULL;

    void * element = ((void **) bufferGet(deque -> buffer, deque -> buffer -> size - 1))[deque -> back];
    deque -> size -= 1;

    if (deque -> back == 0) {
        deque -> back = deque -> capacity - 1;
        deque -> back_empty = true;
    }
    else {
        deque -> back -= 1;
        deque -> back_empty = false;
    }
    
    if (deque -> size == 1)
        deque -> front = deque -> back;

    if (deque -> size == 0)
        free(bufferPopBack(deque -> buffer));

    return element;

exit:
    fprintf(stderr, "File: %s.\nOperation: dequePopBack.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Pops the element at the front of the deque.
 *
 * @param       deque pointer to deque to pop front from.
 *
 * @return      the element at the front of the deque.
 */
void * dequePopFront(struct Deque * const deque) {
    char const * message = NULL;

    if (deque == NULL) {
        message = "The parameter <deque> cannot be NULL.";
        goto exit;
    }
    
    if (deque -> front_empty && deque -> buffer -> size > 1) {
        free(bufferPopFront(deque -> buffer));
        deque -> front_empty = false;
    }

    if (deque -> size == 0)
        return NULL;

    void * element = ((void **) bufferGet(deque -> buffer, 0))[deque -> front];
    deque -> size -= 1;

    if (deque -> front == deque -> capacity - 1) {
        deque -> front = 0;
        deque -> front_empty = true;
    }
    else {
        deque -> front += 1;
        deque -> front_empty = false;
    }

    if (deque -> size == 1)
        deque -> back = deque -> front;

    // If we exhausted all the elements, we delete the current front index buffer
    if (deque -> size == 0)
        free(bufferPopFront(deque -> buffer));

    return element;

exit:
    fprintf(stderr, "File: %s.\nOperation: dequePopFront.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Gets an element at the back of the deque.
 *
 * @param       deque pointer to deque to get the back element from.
 *
 * @return      the element at the back of the deque.
 */
void * dequePeekBack(struct Deque * const deque) {
    char const * message = "The parameter <deque> cannot be NULL.";
    if (deque == NULL)
        goto exit;

    if (deque -> size == 0)
        return NULL;
    
    return ((void **) bufferGet(deque -> buffer, deque -> buffer -> size - 1))[deque -> back];

exit:
    fprintf(stderr, "File: %s.\nOperation: dequePeekBack.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Gets the element at the front of the deque.
 *
 * @param       deque pointer to deque to get the front element from.
 *
 * @return      the element at the front of the deque.
 */
void * dequePeekFront(struct Deque * const deque) {
    char const * message = "The parameter <deque> cannot be NULL.";
    if (deque == NULL)
        goto exit;

    if (deque -> size == 0)
        return NULL;
    
    return ((void **) bufferGet(deque -> buffer, 0))[deque -> front];

exit:
    fprintf(stderr, "File: %s.\nOperation: dequePeekFront.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Gets the element at the front of the deque.
 *
 * @param       deque pointer to deque to get the front element from.
 *
 * @return      the element at the front of the deque.
 */
void * dequeGet(struct Deque * const deque, unsigned index) {
    char const * message = NULL;

    if (deque == NULL) {
        message = "The parameter <deque> cannot be NULL.";
        goto exit;
    }

    if (deque -> size == 0)
        return NULL;

    if (index >= deque -> size) {
        message = "Index is out of bounds.";
        goto exit;
    }

    unsigned pos = index + deque -> front;
    return ((void **) bufferGet(deque -> buffer, (pos / deque -> capacity) + (deque -> front_empty ? 1 : 0)))[pos % deque -> capacity];

exit:
    fprintf(stderr, "File: %s.\nOperation: dequeGet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Sets the element at the given position in the deque.
 *
 * @param       deque pointer to deque to get the element from.
 */
void dequeSet(struct Deque * const deque, unsigned index, void * element) {
    char const * message = NULL;

    if (deque == NULL) {
        message = "The parameter <deque> cannot be NULL.";
        goto exit;
    }

    if (deque -> size == 0) {
        message = "Cannot set onto an empty deque.";
        goto exit;
    }

    if (index >= deque -> size) {
        message = "Index is out of bounds.";
        goto exit;
    }

    unsigned pos = index + deque -> front;
    ((void **) bufferGet(deque -> buffer, (pos / deque -> capacity) + (deque -> front_empty ? 1 : 0)))[pos % deque -> capacity] = element;

    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: dequeSet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}



/*
 * /!\
 * Note that in the functions below operating on the buffer we don't have many checks.
 * And that's because this code is internal, and we trust ourselves to not pass NULL pointers around.
 */

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
    memmove(buffer -> contents, buffer -> contents + 1, --buffer -> size * sizeof(void *));
    return content;
}

static inline void * bufferGet(struct Buffer const * const buffer, unsigned index) {
    return buffer -> contents[index];
}
