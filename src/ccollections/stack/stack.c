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

#include "stack.h"

float stack_growth_factor = 1.75;


/**
 * Initializes the stack
 *
 * @return      the newly created stack.
 */
struct Stack * newStack(unsigned initial_capacity) {
    const char * message = "Initial stack size cannot be zero.";
    if(initial_capacity == 0)
        goto exit;

    struct Stack * stack = malloc(sizeof *stack);
    if (stack == NULL)
        return NULL;

    stack -> elements = malloc(initial_capacity * sizeof *stack -> elements);
    if (stack -> elements == NULL) {
        free(stack);
        return NULL;
    }

    struct Collection collection = {
        .get = NULL,
        .set = NULL,
        .atEnd = NULL,
    };

    stack -> collection = collection;
    stack -> capacity = initial_capacity;
    stack -> top = 0;

    return stack;

exit:
    fprintf(stderr, "File: %s.\nOperation: newStack.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Frees the memory occupied by the stack.
 *
 * @param       stack pointer to memory occupied by the stack.
 */
void deleteStack(struct Stack ** const stack, CDeleter deleter) {
    if (stack == NULL)
        return;

    if (* stack == NULL)
        return;

    if (deleter != NULL) {
        while (isStackEmpty(stack) == false) {
            void * element = stackPop(stack);
            deleter(& element);
        }
    }

    free((* stack) -> elements);
    free(* stack);
    * stack = NULL;
}


/**
 * Check if the stack is empty.
 *
 * @param       stack pointer to the stack which content to check.
 *
 * @return      true if the stack is empty, false otherwise.
 */
bool isStackEmpty(struct Stack const * const stack) {
    char const * message = "The parameter <stack> cannot be NULL.";
    if (stack == NULL)
        goto exit;

    return stack -> top == 0;

exit:
    fprintf(stderr, "File: %s.\nOperation: isStackEmpty.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Returns the top of the stack.
 *
 * @param       stack pointer to the stack which content to check.
 *
 * @return      the element at the top of the stack.
 */
void * stackTop(struct Stack const * const stack) {
    char const * message = NULL;
    
    if (stack == NULL) {
        message = "The parameter <stack> cannot be NULL.";
        goto exit;
    }

    if (stack-> top == 0) {
        message = "The stack is empty, cannot get the top element.";
        goto exit;
    }

    return stack -> elements[stack -> top - 1];

exit:
    fprintf(stderr, "File: %s.\nOperation: stackTop.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Push an element on top of the stack.
 *
 * @param       stack pointer to stack to push an element on.
 * @param       element pointer to the element to push onto the stack.
 */
void stackPush(struct Stack * const stack, void * element) {
    char const * message = NULL;
    
    if (stack == NULL) {
        message = "The parameter <stack> cannot be NULL.";
        goto exit;
    }

    if (stack -> top == stack -> capacity) {
        unsigned new_capacity = stack_growth_factor * stack -> capacity;
        void ** new_elements = realloc(stack -> elements, new_capacity * sizeof *stack -> elements);
        if (new_elements == NULL) {
            message = "Failed to allocated space for new elements.";
            goto exit;
        }

        stack -> elements = new_elements;
        stack -> capacity = new_capacity;
    }

    stack -> elements[stack -> top++] = element;

    return;

exit:
    fprintf(stderr, "File: %s.\nOperation: stackPush.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Pop an element off the top of the stack.
 *
 * @param       stack pointer to the stack to pop the element off.
 *
 * @return      pointer to the element popped off the stack top.
 */
void * stackPop(struct Stack * const stack) {
    char const * message = NULL;
    
    if (stack == NULL) {
        message = "The parameter <stack> cannot be NULL.";
        goto exit;
    }

    if (stack -> top == 0) {
        message = "Stack is empty, it cannot be popped.";
        goto exit;
    }

    return stack -> elements[-- (stack -> top)];

exit:
    fprintf(stderr, "File: %s.\nOperation: stackPop.\nMessage: %s\n", __FILE__, message);
    exit(74);
}
