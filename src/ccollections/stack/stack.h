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

#ifndef CCOLLECTIONS_STACK_H
#define CCOLLECTIONS_STACK_H

#include <stdbool.h>
#include <stddef.h>

extern float stack_growth_factor;

struct Stack {
    void ** elements;
    size_t size;
    size_t top;
};


/**
 * Initializes the stack
 *
 * @return      the newly created stack.
 */
struct Stack * newStack(size_t size);


/**
 * Frees the memory occupied by the stack.
 *
 * @param       stack pointer to memory occupied by the stack.
 */
void deleteStack(struct Stack ** const stack);


/**
 * Check if the stack is empty.
 *
 * @param       stack pointer to the stack which content to check.
 *
 * @return      true if the stack is empty, false otherwise.
 */
bool isStackEmpty(struct Stack const * const stack);


/**
 * Returns the top of the stack.
 *
 * @param       stack pointer to the stack which content to check.
 *
 * @return      the element at the top of the stack.
 */
void * stackTop(struct Stack const * const stack);


/**
 * Push an element on top of the stack.
 *
 * @param       stack pointer to stack to push an element on.
 * @param       element pointer to the element to push onto the stack.
 */
void stackPush(struct Stack * const stack, void * element);


/**
 * Pop an element off the top of the stack.
 *
 * @param       stack pointer to the stack to pop the element off.
 *
 * @return      the element popped off the stack top.
 */
void * stackPop(struct Stack * const stack);

#endif
