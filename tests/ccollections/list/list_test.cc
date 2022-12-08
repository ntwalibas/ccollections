#include <gtest/gtest.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
    #include "src/ccollections/list/list.h"
}

class ListTest: public ::testing::Test {
    protected:
        void SetUp() override {
            list = newList();
        }

        void TearDown() override {
            deleteList(&list, nullptr);
        }
    
        struct List * list;
};

// newList
TEST_F(ListTest, newListTest) {
    // Expect that a new list was created
    EXPECT_NE(list, nullptr);
}

// deleteList
TEST_F(ListTest, deleteListTest) {
    deleteList(&list, nullptr);

    // Make sure the list is freed upon calling deleteList
    EXPECT_EQ(list, nullptr);
}

// isListEmpty
TEST_F(ListTest, isListEmptyTest) {
    // No elements have been added to the list, it should be empty
    EXPECT_EQ(isListEmpty(list), true);

    // We delete the list, we should gracefully fail to check if it is empty, without running into null pointer accesses
    deleteList(&list, nullptr);

    const char formatter[] = "File: %s.\nOperation: isListEmpty.\nMessage: %s\n";
    const char file[] = "src/ccollections/list/list.c";
    const char message[] = "The parameter <list> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(isListEmpty(list), expected_message);

    free(expected_message);
}
