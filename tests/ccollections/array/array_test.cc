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

// resizeArray
TEST_F(ArrayTest, resizeArrayTest) {
    array = resizeArray(array, 20);
    EXPECT_EQ(array -> capacity, 20);

    // Check that a capacity less than or equal to the current capacity results in a failure
    const char formatter1[] = "File: %s.\nOperation: resizeArray.\nMessage: %s\n";
    const char file1[] = "src/ccollections/array/array.c";
    const char message1[] = "The new capacity cannot less or equal to the existing capacity.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(resizeArray(array, 20), expected_message1);

    free(expected_message1);

    // We delete the array, we should gracefully fail to check if it is empty, without running into null pointer accesses
    deleteArray(&array);

    const char formatter2[] = "File: %s.\nOperation: resizeArray.\nMessage: %s\n";
    const char file2[] = "src/ccollections/array/array.c";
    const char message2[] = "The parameter <array> cannot be NULL.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(resizeArray(array, 20), expected_message2);

    free(expected_message2);
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

// arrayPushBack
TEST_F(ArrayTest, arrayPushBackTest) {
    // Append an element and verify that it was push
    int value = 1;
    arrayPushBack(array, &value);
    EXPECT_EQ(array -> size, 1);

    // We delete the array, we should not be able to push onto it
    deleteArray(&array);

    const char formatter[] = "File: %s.\nOperation: arrayPushBack.\nMessage: %s\n";
    const char file[] = "src/ccollections/array/array.c";
    const char message[] = "The parameter <array> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(arrayPushBack(array, &value), expected_message);

    free(expected_message);
}

// arrayGet
TEST_F(ArrayTest, arrayGetTest) {
    // No elements have been added to the array, we should not be able to get elements
    const char formatter1[] = "File: %s.\nOperation: _arrayCollectionGet.\nMessage: %s\n";
    const char file1[] = "src/ccollections/array/array.c";
    const char message1[] = "The array is empty, cannot get elements.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(arrayGet(array, 0), expected_message1);

    free(expected_message1);


    // Add an element to the array and verify that we get it at index 0
    int value = 1;
    arrayPushBack(array, &value);
    int * element = (int *) arrayGet(array, 0);
    EXPECT_EQ(element, & value);
    EXPECT_EQ(* element, value);


    // Try to get an element beyond what's been added
    const char formatter2[] = "File: %s.\nOperation: _arrayCollectionGet.\nMessage: %s\n";
    const char file2[] = "src/ccollections/array/array.c";
    const char message2[] = "The index is out of bounds.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(arrayGet(array, 1), expected_message2);

    free(expected_message2);


    // We delete the array, we should not be able to get the top element
    deleteArray(&array);

    const char formatter3[] = "File: %s.\nOperation: _arrayCollectionGet.\nMessage: %s\n";
    const char file3[] = "src/ccollections/array/array.c";
    const char message3[] = "The parameter <array> cannot be NULL.";
    
    int size3 = snprintf(NULL, 0, formatter3, file3, message3);
    char * expected_message3 = (char *) malloc((size3 + 1) * sizeof(char));
    snprintf(expected_message3, size3 + 1, formatter3, file3, message3);

    EXPECT_DEATH(arrayGet(array, 0), expected_message3);

    free(expected_message3);
}

// arraySet
TEST_F(ArrayTest, arraySetTest) {
    // No elements have been added to the array, we should not be able to set elements
    int value1 = 1;
    const char formatter1[] = "File: %s.\nOperation: _arrayCollectionSet.\nMessage: %s\n";
    const char file1[] = "src/ccollections/array/array.c";
    const char message1[] = "The array is empty, cannot set elements.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(arraySet(array, 0, &value1), expected_message1);

    free(expected_message1);


    // Add an element to the array and verify that we set it at index 0
    int value2 = 2;
    arrayPushBack(array, &value1);
    arraySet(array, 0, &value2);
    int * element = (int *) arrayGet(array, 0);
    EXPECT_NE(element, & value1);
    EXPECT_NE(* element, value1);
    EXPECT_EQ(element, & value2);
    EXPECT_EQ(* element, value2);


    // Try to get an element beyond what's been added
    const char formatter2[] = "File: %s.\nOperation: _arrayCollectionSet.\nMessage: %s\n";
    const char file2[] = "src/ccollections/array/array.c";
    const char message2[] = "The index is out of bounds.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(arraySet(array, 1, &value2), expected_message2);

    free(expected_message2);


    // We delete the array, we should not be able to get the top element
    deleteArray(&array);

    const char formatter3[] = "File: %s.\nOperation: _arrayCollectionSet.\nMessage: %s\n";
    const char file3[] = "src/ccollections/array/array.c";
    const char message3[] = "The parameter <array> cannot be NULL.";
    
    int size3 = snprintf(NULL, 0, formatter3, file3, message3);
    char * expected_message3 = (char *) malloc((size3 + 1) * sizeof(char));
    snprintf(expected_message3, size3 + 1, formatter3, file3, message3);

    EXPECT_DEATH(arraySet(array, 0, &value2), expected_message3);

    free(expected_message3);
}
