#include <gtest/gtest.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
    #include "src/ccollections/stack/stack.h"
}

class StackTest: public ::testing::Test {
    protected:
        void SetUp() override {
            stack = newStack(10);
        }

        void TearDown() override {
            deleteStack(&stack);
        }
    
        struct Stack * stack;
};

// newStack
TEST_F(StackTest, newStackTest) {
    // Expect that a new stack was created
    EXPECT_NE(stack, nullptr);

    // Expect that the stack size is equal to 10
    EXPECT_EQ(stack -> size, 10);

    // Check that a different stack with size 0 fails to be created
    const char formatter[] = "File: %s.\nOperation: newStack.\nMessage: %s\n";
    const char file[] = "src/ccollections/stack/stack.c";
    const char message[] = "Initial stack size cannot be zero.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(newStack(0), expected_message);

    free(expected_message);
}

// deleteStack
TEST_F(StackTest, deleteStackTest) {
    deleteStack(&stack);

    // Make sure the stack is freed upon calling deleteStack
    EXPECT_EQ(stack, nullptr);
}

// isStackEmpty
TEST_F(StackTest, isStackEmptyTest) {
    // No elements have been added to the stack, it should be empty
    EXPECT_EQ(isStackEmpty(stack), true);

    // We delete the stack, we should gracefully fail to check if it is empty, without running into null pointer accesses
    deleteStack(&stack);

    const char formatter[] = "File: %s.\nOperation: isStackEmpty.\nMessage: %s\n";
    const char file[] = "src/ccollections/stack/stack.c";
    const char message[] = "The parameter <stack> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(isStackEmpty(stack), expected_message);

    free(expected_message);
}

// stackTop
TEST_F(StackTest, stackTopTest) {
    // No elements have been added to the stack, we should not be able to get the top element
    const char formatter1[] = "File: %s.\nOperation: stackTop.\nMessage: %s\n";
    const char file1[] = "src/ccollections/stack/stack.c";
    const char message1[] = "The stack is empty, cannot get the top element.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(stackTop(stack), expected_message1);

    free(expected_message1);


    // Add an element to the stack and verify that stackTop is indeed the recently pushed element
    int value = 1;
    stackPush(stack, &value);
    int * element = (int *) stackTop(stack);
    EXPECT_EQ(element, & value);
    EXPECT_EQ(* element, value);

    // We delete the stack, we should not be able to get the top element
    deleteStack(&stack);

    const char formatter2[] = "File: %s.\nOperation: stackTop.\nMessage: %s\n";
    const char file2[] = "src/ccollections/stack/stack.c";
    const char message2[] = "The parameter <stack> cannot be NULL.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(stackTop(stack), expected_message2);

    free(expected_message2);
}

// stackPush
TEST_F(StackTest, stackPushTest) {
    // Push an element and verify that it was push
    int value = 1;
    stackPush(stack, &value);
    EXPECT_EQ(stack -> top, 1);

    // We delete the stack, we should not be able to push onto it
    deleteStack(&stack);

    const char formatter[] = "File: %s.\nOperation: stackPush.\nMessage: %s\n";
    const char file[] = "src/ccollections/stack/stack.c";
    const char message[] = "The parameter <stack> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(stackPush(stack, &value), expected_message);

    free(expected_message);
}

// stackPop
TEST_F(StackTest, stackPopTest) {
    // No elements have been added to the stack, we should not be able to pop the top element
    const char formatter1[] = "File: %s.\nOperation: stackPop.\nMessage: %s\n";
    const char file1[] = "src/ccollections/stack/stack.c";
    const char message1[] = "Stack is empty, it cannot be popped.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(stackPop(stack), expected_message1);

    free(expected_message1);

    // Pop an element and verify if it was returned and the top element tracker moved
    int value = 1;
    stackPush(stack, &value);
    int * element = (int *) stackPop(stack);
    EXPECT_EQ(* element, value);
    EXPECT_EQ(stack -> top, 0);

    // We delete the stack, we should not be able to pop the top element
    deleteStack(&stack);

    const char formatter2[] = "File: %s.\nOperation: stackPop.\nMessage: %s\n";
    const char file2[] = "src/ccollections/stack/stack.c";
    const char message2[] = "The parameter <stack> cannot be NULL.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(stackPop(stack), expected_message2);

    free(expected_message2);
}
