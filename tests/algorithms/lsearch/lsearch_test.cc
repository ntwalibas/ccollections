#include <gtest/gtest.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
    #include "lsearch.h"
    #include "vector.h"
}


int int_comparator(void const * a, void const * b) {
    int const * a_int = (const int *) a;
    int const * b_int = (const int *) b;
    return (*a_int > *b_int) - (*a_int < *b_int);
}


class LSearchTest: public ::testing::Test {
    protected:
        void SetUp() override {
            vector = newVector(10);

            elements = (int *) malloc(10 * sizeof(*elements));
            for(int i = 0; i < 10; i++) {
                elements[i] = i + 1;
                vectorPushBack(vector, &elements[i]);
            }
        }

        void TearDown() override {
            deleteVector(&vector, NULL);
            free(elements);
        }
    
        struct Vector * vector;
        int * elements;
};

TEST_F(LSearchTest, lsearchTest) {
    // Verify that an existing element is indeed found at the expected index
    int needle = 4;
    int index = lsearch(&vector -> collection, &needle, &int_comparator);
    EXPECT_EQ(index, 3);

    // Verify that an non-existent element results in -1 being returned
    needle = 11;
    index = lsearch(&vector -> collection, &needle, &int_comparator);
    EXPECT_EQ(index, -1);
}
