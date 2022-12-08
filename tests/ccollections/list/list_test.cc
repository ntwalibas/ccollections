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

    // We delete the list, without running into null pointer accesses
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

// listPushBack
TEST_F(ListTest, listPushBackTest) {
    int values[] = {1, 2, 3, 4, 5, 6};

    // Push onto the list
    listPushBack(list, &values[0]);
    listPushBack(list, &values[1]);
    listPushBack(list, &values[2]);
    listPushBack(list, &values[3]);
    listPushBack(list, &values[4]);
    listPushBack(list, &values[5]);

    EXPECT_EQ(list -> size, 6);
    EXPECT_EQ(*((int *)list -> head -> element), 1);
    EXPECT_EQ(*((int *)list -> tail -> element), 6);

    // We delete the list, without running into null pointer accesses
    deleteList(&list, nullptr);

    const char formatter[] = "File: %s.\nOperation: listPushBack.\nMessage: %s\n";
    const char file[] = "src/ccollections/list/list.c";
    const char message[] = "The parameter <list> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(listPushBack(list, &values[0]), expected_message);

    free(expected_message);
}

// listPushFront
TEST_F(ListTest, listPushFrontTest) {
    int values[] = {1, 2, 3, 4, 5, 6};

    // Push onto the list
    listPushFront(list, &values[0]);
    listPushFront(list, &values[1]);
    listPushFront(list, &values[2]);
    listPushFront(list, &values[3]);
    listPushFront(list, &values[4]);
    listPushFront(list, &values[5]);

    EXPECT_EQ(list -> size, 6);
    EXPECT_EQ(*((int *)list -> head -> element), 6);
    EXPECT_EQ(*((int *)list -> tail -> element), 1);

    // We delete the list, without running into null pointer accesses
    deleteList(&list, nullptr);

    const char formatter[] = "File: %s.\nOperation: listPushFront.\nMessage: %s\n";
    const char file[] = "src/ccollections/list/list.c";
    const char message[] = "The parameter <list> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(listPushFront(list, &values[0]), expected_message);

    free(expected_message);
}

// listPopBack
TEST_F(ListTest, listPopBackTest) {
    int values[] = {1, 2, 3, 4, 5, 6};

    // Push onto the list
    listPushBack(list, &values[0]);
    listPushBack(list, &values[1]);
    listPushBack(list, &values[2]);
    listPushBack(list, &values[3]);
    listPushBack(list, &values[4]);
    listPushBack(list, &values[5]);

    EXPECT_EQ(*((int *)listPopBack(list)), 6);
    EXPECT_EQ(list -> size, 5);
    EXPECT_EQ(*((int *)list -> tail -> element), 5);

    EXPECT_EQ(*((int *)listPopBack(list)), 5);
    EXPECT_EQ(list -> size, 4);
    EXPECT_EQ(*((int *)list -> tail -> element), 4);

    // We delete the list, without running into null pointer accesses
    deleteList(&list, nullptr);

    const char formatter[] = "File: %s.\nOperation: listPopBack.\nMessage: %s\n";
    const char file[] = "src/ccollections/list/list.c";
    const char message[] = "The parameter <list> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(listPopBack(list), expected_message);

    free(expected_message);
}

// listPopFront
TEST_F(ListTest, listPopFrontTest) {
    int values[] = {1, 2, 3, 4, 5, 6};

    // Push onto the list
    listPushBack(list, &values[0]);
    listPushBack(list, &values[1]);
    listPushBack(list, &values[2]);
    listPushBack(list, &values[3]);
    listPushBack(list, &values[4]);
    listPushBack(list, &values[5]);

    EXPECT_EQ(*((int *)listPopFront(list)), 1);
    EXPECT_EQ(list -> size, 5);
    EXPECT_EQ(*((int *)list -> head -> element), 2);
    EXPECT_EQ(*((int *)list -> tail -> element), 6);

    EXPECT_EQ(*((int *)listPopFront(list)), 2);
    EXPECT_EQ(list -> size, 4);
    EXPECT_EQ(*((int *)list -> head -> element), 3);
    EXPECT_EQ(*((int *)list -> tail -> element), 6);

    // We delete the list, without running into null pointer accesses
    deleteList(&list, nullptr);

    const char formatter[] = "File: %s.\nOperation: listPopFront.\nMessage: %s\n";
    const char file[] = "src/ccollections/list/list.c";
    const char message[] = "The parameter <list> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(listPopFront(list), expected_message);

    free(expected_message);
}

// listBack
TEST_F(ListTest, listBackTest) {
    int values[] = {1, 2, 3, 4, 5, 6};

    // Push onto the list
    listPushBack(list, &values[0]);
    listPushBack(list, &values[1]);
    listPushBack(list, &values[2]);
    listPushBack(list, &values[3]);
    listPushBack(list, &values[4]);
    listPushBack(list, &values[5]);

    EXPECT_EQ(*((int *)listBack(list)), 6);
    EXPECT_EQ(list -> size, 6);
    EXPECT_EQ(*((int *)list -> tail -> element), 6);

    // We delete the list, without running into null pointer accesses
    deleteList(&list, nullptr);

    const char formatter[] = "File: %s.\nOperation: listBack.\nMessage: %s\n";
    const char file[] = "src/ccollections/list/list.c";
    const char message[] = "The parameter <list> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(listBack(list), expected_message);

    free(expected_message);
}

// listFront
TEST_F(ListTest, listFrontTest) {
    int values[] = {1, 2, 3, 4, 5, 6};

    // Push onto the list
    listPushBack(list, &values[0]);
    listPushBack(list, &values[1]);
    listPushBack(list, &values[2]);
    listPushBack(list, &values[3]);
    listPushBack(list, &values[4]);
    listPushBack(list, &values[5]);

    EXPECT_EQ(*((int *)listFront(list)), 1);
    EXPECT_EQ(list -> size, 6);
    EXPECT_EQ(*((int *)list -> head -> element), 1);

    // We delete the list, without running into null pointer accesses
    deleteList(&list, nullptr);

    const char formatter[] = "File: %s.\nOperation: listFront.\nMessage: %s\n";
    const char file[] = "src/ccollections/list/list.c";
    const char message[] = "The parameter <list> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(listFront(list), expected_message);

    free(expected_message);
}

// listGet
TEST_F(ListTest, listGetTest) {
    int values[] = {1, 2, 3, 4, 5, 6};

    // Push onto the list
    listPushBack(list, &values[0]);
    listPushBack(list, &values[1]);
    listPushBack(list, &values[2]);
    listPushBack(list, &values[3]);
    listPushBack(list, &values[4]);
    listPushBack(list, &values[5]);

    EXPECT_EQ(*((int *)listGet(list, 0)), 1);
    EXPECT_EQ(*((int *)listGet(list, 4)), 5);
    EXPECT_EQ(*((int *)listGet(list, 1)), 2);
    EXPECT_EQ(*((int *)list -> head -> element), 1);
    EXPECT_EQ(*((int *)list -> tail -> element), 6);


    // Inde is out of bounds
    const char formatter1[] = "File: %s.\nOperation: _listCollectionGet.\nMessage: %s\n";
    const char file1[] = "src/ccollections/list/list.c";
    const char message1[] = "Index is out of bounds.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(listGet(list, -1), expected_message1);

    free(expected_message1);


    // We delete the list, without running into null pointer accesses
    deleteList(&list, nullptr);

    const char formatter2[] = "File: %s.\nOperation: _listCollectionGet.\nMessage: %s\n";
    const char file2[] = "src/ccollections/list/list.c";
    const char message2[] = "The parameter <list> cannot be NULL.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(listGet(list, 0), expected_message2);

    free(expected_message2);
}

// listSet
TEST_F(ListTest, listSetTest) {
    int values[] = {1, 2, 3, 4, 5, 6};
    int replacements[] = {-1, -2, -3};

    // Push onto the list
    listPushBack(list, &values[0]);
    listPushBack(list, &values[1]);
    listPushBack(list, &values[2]);
    listPushBack(list, &values[3]);
    listPushBack(list, &values[4]);
    listPushBack(list, &values[5]);

    listSet(list, 0, &replacements[0]);
    EXPECT_EQ(*((int *)listGet(list, 0)), -1);

    listSet(list, 4, &replacements[1]);
    EXPECT_EQ(*((int *)listGet(list, 4)), -2);

    listSet(list, 1, &replacements[2]);
    EXPECT_EQ(*((int *)listGet(list, 1)), -3);

    EXPECT_EQ(*((int *)list -> head -> element), -1);
    EXPECT_EQ(*((int *)list -> tail -> element), 6);


    // Inde is out of bounds
    const char formatter1[] = "File: %s.\nOperation: _listCollectionSet.\nMessage: %s\n";
    const char file1[] = "src/ccollections/list/list.c";
    const char message1[] = "Index is out of bounds.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(listSet(list, -1, &values[0]), expected_message1);

    free(expected_message1);


    // We delete the list, without running into null pointer accesses
    deleteList(&list, nullptr);

    const char formatter2[] = "File: %s.\nOperation: _listCollectionSet.\nMessage: %s\n";
    const char file2[] = "src/ccollections/list/list.c";
    const char message2[] = "The parameter <list> cannot be NULL.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(listSet(list, 0, &values[0]), expected_message2);

    free(expected_message2);
}

// listInsert
TEST_F(ListTest, listInsertTest) {
    int values[] = {1, 2, 3, 4, 5, 6};
    int replacements[] = {-1, -2, -3};

    // Push onto the list
    listPushBack(list, &values[0]);
    listPushBack(list, &values[1]);
    listPushBack(list, &values[2]);
    listPushBack(list, &values[3]);
    listPushBack(list, &values[4]);
    listPushBack(list, &values[5]);

    listInsert(list, 0, &replacements[0]);
    EXPECT_EQ(*((int *)listGet(list, 0)), -1);
    EXPECT_EQ(*((int *)listGet(list, 1)), 1);

    listInsert(list, 4, &replacements[1]);
    EXPECT_EQ(*((int *)listGet(list, 4)), -2);
    EXPECT_EQ(*((int *)listGet(list, 5)), 4);

    listInsert(list, 1, &replacements[2]);
    EXPECT_EQ(*((int *)listGet(list, 1)), -3);
    EXPECT_EQ(*((int *)listGet(list, 2)), 1);

    EXPECT_EQ(*((int *)list -> head -> element), -1);
    EXPECT_EQ(*((int *)list -> tail -> element), 6);


    // Inde is out of bounds
    const char formatter1[] = "File: %s.\nOperation: listInsert.\nMessage: %s\n";
    const char file1[] = "src/ccollections/list/list.c";
    const char message1[] = "Index is out of bounds.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(listInsert(list, -1, &values[0]), expected_message1);

    free(expected_message1);


    // We delete the list, without running into null pointer accesses
    deleteList(&list, nullptr);

    const char formatter2[] = "File: %s.\nOperation: listInsert.\nMessage: %s\n";
    const char file2[] = "src/ccollections/list/list.c";
    const char message2[] = "The parameter <list> cannot be NULL.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(listInsert(list, 0, &values[0]), expected_message2);

    free(expected_message2);
}
