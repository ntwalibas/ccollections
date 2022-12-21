#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
    #include "vector.h"
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
    EXPECT_DEATH(newVector(0), ::testing::HasSubstr("Initial vector capacity cannot be zero."));
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
    EXPECT_DEATH(resizeVector(vector, 20), ::testing::HasSubstr("The new capacity cannot be less or equal to the existing capacity."));

    // We delete the vector, without running into null pointer accesses
    deleteVector(&vector, nullptr);
    EXPECT_DEATH(resizeVector(vector, 20), ::testing::HasSubstr("The parameter <vector> cannot be NULL."));
}

// isVectorEmpty
TEST_F(VectorTest, isVectorEmptyTest) {
    // No elements have been added to the vector, it should be empty
    EXPECT_EQ(isVectorEmpty(vector), true);

    // We delete the vector, without running into null pointer accesses
    deleteVector(&vector, nullptr);
    EXPECT_DEATH(isVectorEmpty(vector), ::testing::HasSubstr("The parameter <vector> cannot be NULL."));
}

// vectorPushBack
TEST_F(VectorTest, vectorPushBackTest) {
    // Append an element and verify that it was push
    int value = 1;
    vectorPushBack(vector, &value);
    EXPECT_EQ(vector -> size, 1);

    // We delete the vector, we should not be able to push onto it
    deleteVector(&vector, nullptr);
    EXPECT_DEATH(
        vectorPushBack(vector, &value), 
        ::testing::HasSubstr("The parameter <vector> cannot be NULL.")
    );
}

// vectorGet
TEST_F(VectorTest, vectorGetTest) {
    // No elements have been added to the vector, we should not be able to get elements
    EXPECT_DEATH(vectorGet(vector, 0), ::testing::HasSubstr("The vector is empty, cannot get elements."));

    // Add an element to the vector and verify that we get it at index 0
    int value = 1;
    vectorPushBack(vector, &value);
    int * element = (int *) vectorGet(vector, 0);
    EXPECT_EQ(element, & value);
    EXPECT_EQ(* element, value);


    // Try to get an element beyond what's been added
    EXPECT_DEATH(vectorGet(vector, 1), ::testing::HasSubstr("The index is out of bounds."));

    // We delete the vector, we should not be able to get the top element
    deleteVector(&vector, nullptr);
    EXPECT_DEATH(vectorGet(vector, 0), ::testing::HasSubstr("The parameter <vector> cannot be NULL."));
}

// vectorSet
TEST_F(VectorTest, vectorSetTest) {
    // No elements have been added to the vector, we should not be able to set elements
    int value1 = 1;
    EXPECT_DEATH(vectorSet(vector, 0, &value1), ::testing::HasSubstr("The vector is empty, cannot set elements."));

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
    EXPECT_DEATH(vectorSet(vector, 1, &value2), ::testing::HasSubstr("The index is out of bounds."));

    // We delete the vector, we should not be able to get the top element
    deleteVector(&vector, nullptr);
    EXPECT_DEATH(vectorSet(vector, 0, &value2), ::testing::HasSubstr("The parameter <vector> cannot be NULL."));
}
