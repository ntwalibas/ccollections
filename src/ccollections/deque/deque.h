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


#ifndef CCOLLECTIONS_DEQUE_H
#define CCOLLECTIONS_DEQUE_H

#include <stdbool.h>
#include <stddef.h>

#include "../interfaces/interfaces.h"


struct Buffer {
    void ** contents;
    unsigned capacity;
    unsigned size;
};

struct Deque {
    struct Collection collection;
    struct Buffer * buffer;
    bool front_empty;
    bool back_empty;
    int front;
    int back;
    int capacity;
    int size;
};


/**
 * Initializes the deque
 *
 * @return      the newly created deque.
 */
struct Deque * newDeque(unsigned capacity);


/**
 * Frees the memory occupied by the deque.
 *
 * @param       deque pointer to memory occupied by the deque.
 */
void deleteDeque(struct Deque ** const deque, CDeleter deleter);


/**
 * Check if the deque is empty.
 *
 * @param       deque pointer to the deque which content to check.
 *
 * @return      true if the deque is empty, false otherwise.
 */
bool isDequeEmpty(struct Deque const * const deque);


/**
 * Check if the deque is full.
 *
 * @param       deque pointer to the deque which content to check.
 *
 * @return      true if the deque is full, false otherwise.
 */
bool isDequeFull(struct Deque const * const deque);


/**
 * Pushes an element to the back of the deque.
 *
 * @param       deque pointer to deque to push back onto.
 * @param       element pointer to the element to push back.
 */
void dequePushBack(struct Deque * const deque, void * element);


/**
 * Pushes the element to the front of the deque.
 *
 * @param       deque pointer to deque to push front onto.
 * @param       element pointer to the element to push at the front.
 */
void dequePushFront(struct Deque * const deque, void * element);


/**
 * Pops an element at the back of the deque.
 *
 * @param       deque pointer to deque to pop back from.
 *
 * @return      the element at the back of the deque.
 */
void * dequePopBack(struct Deque * const deque);


/**
 * Pops the element at the front of the deque.
 *
 * @param       deque pointer to deque to pop front from.
 *
 * @return      the element at the front of the deque.
 */
void * dequePopFront(struct Deque * const deque);


/**
 * Gets an element at the back of the deque.
 *
 * @param       deque pointer to deque to get the back element from.
 *
 * @return      the element at the back of the deque.
 */
void * dequePeekBack(struct Deque const * const deque);


/**
 * Gets the element at the front of the deque.
 *
 * @param       deque pointer to deque to get the front element from.
 *
 * @return      the element at the front of the deque.
 */
void * dequePeekFront(struct Deque const * const deque);


/**
 * Gets the element at the front of the deque.
 *
 * @param       deque pointer to deque to get the front element from.
 *
 * @return      the element at the front of the deque.
 */
void * dequeGet(struct Deque * const deque, unsigned index);


/**
 * Sets the element at the given position in the deque.
 *
 * @param       deque pointer to deque to get the element from.
 */
void dequeSet(struct Deque * const deque, unsigned index, void * element);

#endif
