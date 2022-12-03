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

    // Expect that the array capacity is equal to 10
    EXPECT_EQ(array -> capacity, 10);

    // Check that a different array with size 0 fails to be created
    const char formatter[] = "File: %s.\nOperation: newArray.\nMessage: %s\n";
    const char file[] = "src/ccollections/array/array.c";
    const char message[] = "Initial array capacity cannot be zero.";
    
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

// isArrayEmpty
TEST_F(ArrayTest, isArrayEmptyTest) {
    // No elements have been added to the array, it should be empty
    EXPECT_EQ(isArrayEmpty(array), true);

    // We delete the array, we should gracefully fail to check if it is empty, without running into null pointer accesses
    deleteArray(&array);

    const char formatter[] = "File: %s.\nOperation: isArrayEmpty.\nMessage: %s\n";
    const char file[] = "src/ccollections/array/array.c";
    const char message[] = "The parameter <array> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(isArrayEmpty(array), expected_message);

    free(expected_message);
}

// arrayAppend
TEST_F(ArrayTest, arrayAppendTest) {
    // Append an element and verify that it was push
    int value = 1;
    arrayAppend(array, &value);
    EXPECT_EQ(array -> size, 1);

    // We delete the array, we should not be able to push onto it
    deleteArray(&array);

    const char formatter[] = "File: %s.\nOperation: arrayAppend.\nMessage: %s\n";
    const char file[] = "src/ccollections/array/array.c";
    const char message[] = "The parameter <array> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(arrayAppend(array, &value), expected_message);

    free(expected_message);
}
