#include <gtest/gtest.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
    #include "src/ccollections/vector/vector.h"
}

class VectorTest: public ::testing::Test {
    protected:
        void SetUp() override {
            vector = newVector(10);
        }

        void TearDown() override {
            deleteVector(&vector, nullptr);
        }
    
        struct Vector * vector;
};

// newVector
TEST_F(VectorTest, newVectorTest) {
    // Expect that a new vector was created
    EXPECT_NE(vector, nullptr);

    // Expect that the vector capacity is equal to 10
    EXPECT_EQ(vector -> capacity, 10);

    // Check that a different vector with size 0 fails to be created
    const char formatter[] = "File: %s.\nOperation: newVector.\nMessage: %s\n";
    const char file[] = "src/ccollections/vector/vector.c";
    const char message[] = "Initial vector capacity cannot be zero.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(newVector(0), expected_message);

    free(expected_message);
}

// deleteVector
TEST_F(VectorTest, deleteVectorTest) {
    deleteVector(&vector, nullptr);

    // Make sure the vector is freed upon calling deleteVector
    EXPECT_EQ(vector, nullptr);
}

// resizeVector
TEST_F(VectorTest, resizeVectorTest) {
    vector = resizeVector(vector, 20);
    EXPECT_EQ(vector -> capacity, 20);

    // Check that a capacity less than or equal to the current capacity results in a failure
    const char formatter1[] = "File: %s.\nOperation: resizeVector.\nMessage: %s\n";
    const char file1[] = "src/ccollections/vector/vector.c";
    const char message1[] = "The new capacity cannot less or equal to the existing capacity.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(resizeVector(vector, 20), expected_message1);

    free(expected_message1);

    // We delete the vector, we should gracefully fail to check if it is empty, without running into null pointer accesses
    deleteVector(&vector, nullptr);

    const char formatter2[] = "File: %s.\nOperation: resizeVector.\nMessage: %s\n";
    const char file2[] = "src/ccollections/vector/vector.c";
    const char message2[] = "The parameter <vector> cannot be NULL.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(resizeVector(vector, 20), expected_message2);

    free(expected_message2);
}

// isVectorEmpty
TEST_F(VectorTest, isVectorEmptyTest) {
    // No elements have been added to the vector, it should be empty
    EXPECT_EQ(isVectorEmpty(vector), true);

    // We delete the vector, we should gracefully fail to check if it is empty, without running into null pointer accesses
    deleteVector(&vector, nullptr);

    const char formatter[] = "File: %s.\nOperation: isVectorEmpty.\nMessage: %s\n";
    const char file[] = "src/ccollections/vector/vector.c";
    const char message[] = "The parameter <vector> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(isVectorEmpty(vector), expected_message);

    free(expected_message);
}

// vectorPushBack
TEST_F(VectorTest, vectorPushBackTest) {
    // Append an element and verify that it was push
    int value = 1;
    vectorPushBack(vector, &value);
    EXPECT_EQ(vector -> size, 1);

    // We delete the vector, we should not be able to push onto it
    deleteVector(&vector, nullptr);

    const char formatter[] = "File: %s.\nOperation: vectorPushBack.\nMessage: %s\n";
    const char file[] = "src/ccollections/vector/vector.c";
    const char message[] = "The parameter <vector> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(vectorPushBack(vector, &value), expected_message);

    free(expected_message);
}

// vectorGet
TEST_F(VectorTest, vectorGetTest) {
    // No elements have been added to the vector, we should not be able to get elements
    const char formatter1[] = "File: %s.\nOperation: _vectorCollectionGet.\nMessage: %s\n";
    const char file1[] = "src/ccollections/vector/vector.c";
    const char message1[] = "The vector is empty, cannot get elements.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(vectorGet(vector, 0), expected_message1);

    free(expected_message1);


    // Add an element to the vector and verify that we get it at index 0
    int value = 1;
    vectorPushBack(vector, &value);
    int * element = (int *) vectorGet(vector, 0);
    EXPECT_EQ(element, & value);
    EXPECT_EQ(* element, value);


    // Try to get an element beyond what's been added
    const char formatter2[] = "File: %s.\nOperation: _vectorCollectionGet.\nMessage: %s\n";
    const char file2[] = "src/ccollections/vector/vector.c";
    const char message2[] = "The index is out of bounds.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(vectorGet(vector, 1), expected_message2);

    free(expected_message2);


    // We delete the vector, we should not be able to get the top element
    deleteVector(&vector, nullptr);

    const char formatter3[] = "File: %s.\nOperation: _vectorCollectionGet.\nMessage: %s\n";
    const char file3[] = "src/ccollections/vector/vector.c";
    const char message3[] = "The parameter <vector> cannot be NULL.";
    
    int size3 = snprintf(NULL, 0, formatter3, file3, message3);
    char * expected_message3 = (char *) malloc((size3 + 1) * sizeof(char));
    snprintf(expected_message3, size3 + 1, formatter3, file3, message3);

    EXPECT_DEATH(vectorGet(vector, 0), expected_message3);

    free(expected_message3);
}

// vectorSet
TEST_F(VectorTest, vectorSetTest) {
    // No elements have been added to the vector, we should not be able to set elements
    int value1 = 1;
    const char formatter1[] = "File: %s.\nOperation: _vectorCollectionSet.\nMessage: %s\n";
    const char file1[] = "src/ccollections/vector/vector.c";
    const char message1[] = "The vector is empty, cannot set elements.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(vectorSet(vector, 0, &value1), expected_message1);

    free(expected_message1);


    // Add an element to the vector and verify that we set it at index 0
    int value2 = 2;
    vectorPushBack(vector, &value1);
    vectorSet(vector, 0, &value2);
    int * element = (int *) vectorGet(vector, 0);
    EXPECT_NE(element, & value1);
    EXPECT_NE(* element, value1);
    EXPECT_EQ(element, & value2);
    EXPECT_EQ(* element, value2);


    // Try to get an element beyond what's been added
    const char formatter2[] = "File: %s.\nOperation: _vectorCollectionSet.\nMessage: %s\n";
    const char file2[] = "src/ccollections/vector/vector.c";
    const char message2[] = "The index is out of bounds.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(vectorSet(vector, 1, &value2), expected_message2);

    free(expected_message2);


    // We delete the vector, we should not be able to get the top element
    deleteVector(&vector, nullptr);

    const char formatter3[] = "File: %s.\nOperation: _vectorCollectionSet.\nMessage: %s\n";
    const char file3[] = "src/ccollections/vector/vector.c";
    const char message3[] = "The parameter <vector> cannot be NULL.";
    
    int size3 = snprintf(NULL, 0, formatter3, file3, message3);
    char * expected_message3 = (char *) malloc((size3 + 1) * sizeof(char));
    snprintf(expected_message3, size3 + 1, formatter3, file3, message3);

    EXPECT_DEATH(vectorSet(vector, 0, &value2), expected_message3);

    free(expected_message3);
}
