#include <gtest/gtest.h>
#include <stddef.h>

extern "C" {
    #include "src/ccollections/stack/stack.h"
}


// Make sure a stack of the given size is created upon request
TEST(StackTestSuite, newStackTest) {
    char const * message = "Stack initialization failed.";
    struct Stack * stack = newStack(10, message);
    // Expect that a new stack was created
    EXPECT_NE(stack, nullptr);

    // Expect that the stack size is equal to 10
    EXPECT_EQ(stack -> size, 10);

    // Expect that in case of failure, the correct message was set
    EXPECT_STREQ(stack -> message, message);
}
