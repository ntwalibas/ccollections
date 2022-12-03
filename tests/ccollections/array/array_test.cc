#include <gtest/gtest.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
    #include "src/ccollections/array/array.h"
}

class ArrayTest: public ::testing::Test {
    protected:
        void SetUp() override {
            array = newArray(10);
        }

        void TearDown() override {
            deleteArray(&array);
        }
    
        struct Array * array;
};

// newArray
TEST_F(ArrayTest, newArrayTest) {
    // Expect that a new array was created
    EXPECT_NE(array, nullptr);

    // Expect that the array size is equal to 10
    EXPECT_EQ(array -> size, 10);

    // Check that a different array with size 0 fails to be created
    const char formatter[] = "File: %s.\nOperation: newArray.\nMessage: %s\n";
    const char file[] = "src/ccollections/array/array.c";
    const char message[] = "Initial array size cannot be zero.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(newArray(0), expected_message);

    free(expected_message);
}

// deleteArray
TEST_F(ArrayTest, deleteArrayTest) {
    deleteArray(&array);

    // Make sure the array is freed upon calling deleteArray
    EXPECT_EQ(array, nullptr);
}