#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
    #include "deque.h"
}

class DequeTest: public ::testing::Test {
    protected:
        void SetUp() override {
            deque = newDeque(4);
        }

        void TearDown() override {
            deleteDeque(&deque, nullptr);
        }
    
        struct Deque * deque;
};

// newDeque
TEST_F(DequeTest, newDequeTest) {
    // Expect that a new deque was created
    EXPECT_NE(deque, nullptr);

    // Expect that the deque capacity is equal to 10
    EXPECT_EQ(deque -> capacity, 4);

    // Check that a different deque with size 0 fails to be created
    EXPECT_DEATH(newDeque(0), ::testing::HasSubstr("Deque capacity cannot be zero."));
}

// deleteDeque
TEST_F(DequeTest, deleteDequeTest) {
    deleteDeque(&deque, nullptr);

    // Make sure the deque is freed upon calling deleteDeque
    EXPECT_EQ(deque, nullptr);
}

// isDequeEmpty
TEST_F(DequeTest, isDequeEmptyTest) {
    // No elements have been added to the deque, it should be empty
    EXPECT_EQ(isDequeEmpty(deque), true);

    // We add elements, should not be empty
    int values[] = {1};
    dequePushBack(deque, &values[0]);
    EXPECT_EQ(isDequeEmpty(deque), false);

    // We delete the deque, without running into null pointer accesses
    deleteDeque(&deque, nullptr);
    EXPECT_DEATH(isDequeEmpty(deque), ::testing::HasSubstr("The parameter <deque> cannot be NULL."));
}

// isDequeFull
TEST_F(DequeTest, isDequeFullTest) {
    EXPECT_EQ(isDequeFull(deque), false);
    
    // We add elements up to capacity
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8};
    dequePushBack(deque, &values[0]);
    dequePushBack(deque, &values[1]);
    dequePushBack(deque, &values[2]);
    dequePushBack(deque, &values[3]);
    EXPECT_EQ(isDequeFull(deque), true);

    // We add a few more elements leaving one buffer note filled
    dequePushBack(deque, &values[4]);
    dequePushBack(deque, &values[5]);
    EXPECT_EQ(isDequeFull(deque), false);

    // We fill the next couple of missing elements in the new buffer and the deque should be full again
    dequePushBack(deque, &values[6]);
    dequePushBack(deque, &values[7]);
    EXPECT_EQ(isDequeFull(deque), true);

    // We delete the deque, we should gracefully fail to check if it is full, without running into null pointer accesses
    deleteDeque(&deque, nullptr);
    EXPECT_DEATH(isDequeFull(deque), ::testing::HasSubstr("The parameter <deque> cannot be NULL."));
}

// dequePushBack
TEST_F(DequeTest, dequePushBackTest) {
    // Push back an element and verify that it was push
    int values[] = {1, 2, 3, 4, 5, 6, 7};
    dequePushBack(deque, &values[0]);
    dequePushBack(deque, &values[1]);
    dequePushBack(deque, &values[2]);
    dequePushBack(deque, &values[3]);
    dequePushBack(deque, &values[4]);
    dequePushBack(deque, &values[5]);
    dequePushBack(deque, &values[6]);

    EXPECT_EQ(deque -> size, 7);
    EXPECT_EQ(deque -> buffer -> size, 2);

    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[0])[0]), 1);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[0])[1]), 2);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[0])[2]), 3);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[0])[3]), 4);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[1])[0]), 5);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[1])[1]), 6);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[1])[2]), 7);

    // We delete the deque, we should not be able to push onto it
    deleteDeque(&deque, nullptr);
    EXPECT_DEATH(dequePushBack(deque, &values[0]), ::testing::HasSubstr("The parameter <deque> cannot be NULL."));
}

// dequePushFront
TEST_F(DequeTest, dequePushFrontTest) {
    // Push front an element and verify that it was push
    int values[] = {1, 2, 3, 4, 5, 6, 7};
    dequePushFront(deque, &values[0]);
    dequePushFront(deque, &values[1]);
    dequePushFront(deque, &values[2]);
    dequePushFront(deque, &values[3]);
    dequePushFront(deque, &values[4]);
    dequePushFront(deque, &values[5]);
    dequePushFront(deque, &values[6]);

    EXPECT_EQ(deque -> size, 7);
    EXPECT_EQ(deque -> buffer -> size, 2);

    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[1])[3]), 1);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[1])[2]), 2);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[1])[1]), 3);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[1])[0]), 4);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[0])[3]), 5);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[0])[2]), 6);
    EXPECT_EQ(*((int *)((void **)deque -> buffer -> contents[0])[1]), 7);

    // We delete the deque, we should not be able to push onto it
    deleteDeque(&deque, nullptr);
    EXPECT_DEATH(dequePushFront(deque, &values[0]), ::testing::HasSubstr("The parameter <deque> cannot be NULL."));
}

// dequePopBack
TEST_F(DequeTest, dequePopBackTest) {
    // With elements pushed
    int values[] = {1, 2, 3, 4, 5, 6};
    dequePushBack(deque, &values[0]);
    dequePushBack(deque, &values[1]);
    dequePushBack(deque, &values[2]);
    dequePushBack(deque, &values[3]);
    dequePushBack(deque, &values[4]);
    dequePushBack(deque, &values[5]);

    EXPECT_EQ(deque -> size, 6);
    EXPECT_EQ(deque -> buffer -> size, 2);

    EXPECT_EQ(*((int *)(dequePopBack(deque))), 6);
    EXPECT_EQ(*((int *)(dequePopBack(deque))), 5);
    EXPECT_EQ(*((int *)(dequePopBack(deque))), 4);

    EXPECT_EQ(deque -> size, 3);
    EXPECT_EQ(deque -> buffer -> size, 1);

    EXPECT_EQ(*((int *)(dequePopBack(deque))), 3);
    EXPECT_EQ(*((int *)(dequePopBack(deque))), 2);
    EXPECT_EQ(*((int *)(dequePopBack(deque))), 1);

    EXPECT_EQ(deque -> size, 0);
    EXPECT_EQ(deque -> back_empty, true);
    EXPECT_EQ(deque -> buffer -> size, 0);


    // We delete the deque, we should not be able to pop from it
    deleteDeque(&deque, nullptr);
    EXPECT_DEATH(dequePopBack(deque), ::testing::HasSubstr("The parameter <deque> cannot be NULL."));
}

// dequePopFront
TEST_F(DequeTest, dequePopFrontTest) {
    // With elements pushed
    int values[] = {1, 2, 3, 4, 5, 6};
    dequePushFront(deque, &values[0]);
    dequePushFront(deque, &values[1]);
    dequePushFront(deque, &values[2]);
    dequePushFront(deque, &values[3]);
    dequePushFront(deque, &values[4]);
    dequePushFront(deque, &values[5]);

    EXPECT_EQ(deque -> size, 6);
    EXPECT_EQ(deque -> buffer -> size, 2);

    EXPECT_EQ(*((int *)(dequePopFront(deque))), 6);
    EXPECT_EQ(*((int *)(dequePopFront(deque))), 5);
    EXPECT_EQ(*((int *)(dequePopFront(deque))), 4);

    EXPECT_EQ(deque -> size, 3);
    EXPECT_EQ(deque -> front_empty, false);
    EXPECT_EQ(deque -> buffer -> size, 1);

    EXPECT_EQ(*((int *)(dequePopFront(deque))), 3);
    EXPECT_EQ(*((int *)(dequePopFront(deque))), 2);
    EXPECT_EQ(*((int *)(dequePopFront(deque))), 1);

    EXPECT_EQ(deque -> size, 0);
    EXPECT_EQ(deque -> front_empty, true);
    EXPECT_EQ(deque -> buffer -> size, 0);


    // We delete the deque, we should not be able to pop from it
    deleteDeque(&deque, nullptr);
    EXPECT_DEATH(dequePopFront(deque), ::testing::HasSubstr("The parameter <deque> cannot be NULL."));
}

// dequeBack
TEST_F(DequeTest, dequeBackTest) {
    // The deque is empty, we should expect NULL
    EXPECT_EQ(dequeBack(deque), nullptr);

    // With elements pushed
    int values[] = {1, 2};
    dequePushBack(deque, &values[0]);
    dequePushFront(deque, &values[1]);

    EXPECT_EQ(*((int *)(dequeBack(deque))), 1);


    // We delete the deque, we should not be able to pop from it
    deleteDeque(&deque, nullptr);
    EXPECT_DEATH(dequeBack(deque), ::testing::HasSubstr("The parameter <deque> cannot be NULL."));
}

// dequeFront
TEST_F(DequeTest, dequeFrontTest) {
    // The deque is empty, we should expect NULL
    EXPECT_EQ(dequeFront(deque), nullptr);

    // With elements pushed
    int values[] = {1, 2};
    dequePushFront(deque, &values[0]);
    dequePushFront(deque, &values[1]);

    EXPECT_EQ(*((int *)(dequeFront(deque))), 2);


    // We delete the deque, we should not be able to pop from it
    deleteDeque(&deque, nullptr);
    EXPECT_DEATH(dequeFront(deque), ::testing::HasSubstr("The parameter <deque> cannot be NULL."));
}

// dequeGet
TEST_F(DequeTest, dequeGetTest) {
    // The deque is empty, we should expect NULL
    EXPECT_EQ(dequeGet(deque, 0), nullptr);

    // With elements pushed
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    dequePushBack(deque, &values[0]);
    dequePushBack(deque, &values[1]);
    dequePushBack(deque, &values[2]);
    dequePushBack(deque, &values[3]);
    dequePushBack(deque, &values[4]);
    dequePushBack(deque, &values[5]);
    dequePushBack(deque, &values[6]);
    dequePushBack(deque, &values[7]);
    dequePushBack(deque, &values[8]);

    EXPECT_EQ(deque -> size, 9);

    EXPECT_EQ(*((int *)(dequeGet(deque, 0))), 1);
    EXPECT_EQ(*((int *)(dequeGet(deque, 4))), 5);
    EXPECT_EQ(*((int *)(dequeGet(deque, 6))), 7);
    EXPECT_EQ(*((int *)(dequeGet(deque, 8))), 9);

    // We attempt to get an element out of bounds, we should fail
    EXPECT_DEATH(dequeGet(deque, 9), ::testing::HasSubstr("Index is out of bounds."));

    // We delete the deque, we should not be able to get an element from it
    deleteDeque(&deque, nullptr);
    EXPECT_EQ(deque, nullptr);
    EXPECT_DEATH(dequeGet(deque, 0), ::testing::HasSubstr("The parameter <deque> cannot be NULL."));
}

// dequeSet
TEST_F(DequeTest, dequeSetTest) {
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // The deque is empty, We should not be able to set anything
    EXPECT_DEATH(dequeSet(deque, 0, &values[0]), ::testing::HasSubstr("Cannot set onto an empty deque."));

    // With elements pushed
    dequePushBack(deque, &values[0]);
    dequePushBack(deque, &values[1]);
    dequePushBack(deque, &values[2]);
    dequePushBack(deque, &values[3]);
    dequePushBack(deque, &values[4]);
    dequePushBack(deque, &values[5]);

    EXPECT_EQ(*((int *)(dequeGet(deque, 0))), 1);
    dequeSet(deque, 0, &values[8]);
    EXPECT_EQ(*((int *)(dequeGet(deque, 0))), 9);

    EXPECT_EQ(*((int *)(dequeGet(deque, 4))), 5);
    dequeSet(deque, 4, &values[6]);
    EXPECT_EQ(*((int *)(dequeGet(deque, 4))), 7);

    EXPECT_EQ(*((int *)(dequeGet(deque, 3))), 4);
    dequeSet(deque, 3, &values[0]);
    EXPECT_EQ(*((int *)(dequeGet(deque, 3))), 1);

    // We attempt to set an element out of bounds, we should fail
    EXPECT_DEATH(dequeSet(deque, 9, &values[0]), ::testing::HasSubstr("Index is out of bounds."));

    // We delete the deque, we should not be able to set into it
    deleteDeque(&deque, nullptr);
    EXPECT_DEATH(dequeSet(deque, 0, &values[0]), "The parameter <deque> cannot be NULL.");
}
