#include <gtest/gtest.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
    #include "src/ccollections/stack/stack.h"
    #include "src/ccollections/array/array.h"
    #include "src/ccollections/lsearch/lsearch.h"
}


int int_comparator(void const * a, void const * b) {
    int const * a_int = (const int *) a;
    int const * b_int = (const int *) b;
    return (*a_int > *b_int) - (*a_int < *b_int);
}


class LSearchTest: public ::testing::Test {
    protected:
        void SetUp() override {
            array = newArray(10);

            elements = (int *) malloc(10 * sizeof(*elements));
            for(int i = 0; i < 10; i++) {
                elements[i] = i + 1;
                arrayPushBack(array, &elements[i]);
            }
        }

        void TearDown() override {
            deleteArray(&array);
            free(elements);
        }
    
        struct Array * array;
        int * elements;
};

TEST_F(LSearchTest, lsearchTest) {
    // Verify that an existing element is indeed found at the expected index
    int needle = 4;
    int index = lsearch(&array -> collection, &needle, &int_comparator);
    EXPECT_EQ(index, 3);

    // Verify that an non-existent element results in -1 being returned
    needle = 11;
    index = lsearch(&array -> collection, &needle, &int_comparator);
    EXPECT_EQ(index, -1);


    // Verify that a collection without the right virtual functions results in an early termination
    const char formatter1[] = "File: %s.\nOperation: lsearch.\nMessage: %s\n";
    const char file1[] = "src/ccollections/lsearch/lsearch.c";
    const char message1[] = "The collection doesn't provide a mechanism for obtain elements by index/key, and therefore unsearchable.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    struct Stack * stack = newStack(1);
    EXPECT_DEATH(lsearch(&stack -> collection, &needle, &int_comparator), expected_message1);

    free(expected_message1);


    // Verify that given a NULL collection, lsearch fails
    deleteStack(&stack);

    const char formatter2[] = "File: %s.\nOperation: lsearch.\nMessage: %s\n";
    const char file2[] = "src/ccollections/lsearch/lsearch.c";
    const char message2[] = "The parameter <collection> cannot be NULL.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    // Note that we have freed the memory occupied by the stack above so it suffices to pass `stack` being now NULL (and not pass &stack -> collection)
    EXPECT_DEATH(lsearch((struct Collection const * const)stack, &needle, &int_comparator), expected_message2);

    free(expected_message2);
}
