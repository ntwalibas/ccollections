#include <gtest/gtest.h>
#include <stddef.h>

extern "C" {
    #include "src/ccollections/stack/stack.h"
}

class StackTest: public ::testing::Test {
    protected:
        void SetUp() override {
            char const * message = "Stack initialization failed.";
            stack = newStack(10, message);
        }

        void TearDown() override {
            deleteStack(&stack);
        }
    
        struct Stack * stack;
};

// Make sure a stack of the given size is created upon request
TEST_F(StackTest, newStackTest) {
    // Expect that a new stack was created
    EXPECT_NE(stack, nullptr);

    // Expect that the stack size is equal to 10
    EXPECT_EQ(stack -> size, 10);
}

// Make sure the stack is freed upon calling deleteStack
TEST_F(StackTest, deleteStackTest) {
    deleteStack(&stack);
    EXPECT_EQ(stack, nullptr);
}
