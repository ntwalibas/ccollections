#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

extern "C" {
    #include "src/ccollections/set/hashset.h"
    #include "src/ccollections/set/siphash.h"
}

class HashSetTest: public ::testing::Test {
    protected:
        void SetUp() override {
            hash_set = newHashSet(10);
        }

        void TearDown() override {
            deleteHashSet(&hash_set, nullptr);
        }
    
        struct HashSet * hash_set;
};

// newHashSet
TEST_F(HashSetTest, newHashSetTest) {
    // Expect that a new hashSet was created
    EXPECT_NE(hash_set, nullptr);

    // Expect that the hashSet capacity is equal to 10
    EXPECT_EQ(hash_set -> capacity, 10);

    // Check that a different hashSet with size 0 fails to be created
    const char formatter[] = "File: %s.\nOperation: newHashSet.\nMessage: %s\n";
    const char file[] = "src/ccollections/set/hashset.c";
    const char message[] = "Initial hash set capacity cannot be zero.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(newHashSet(0), expected_message);

    free(expected_message);
}

// deleteHashSet
TEST_F(HashSetTest, deleteHashSetTest) {
    deleteHashSet(&hash_set, nullptr);

    // Make sure the hashSet is freed upon calling deleteHashSet
    EXPECT_EQ(hash_set, nullptr);
}

// resizeHashSet
TEST_F(HashSetTest, resizeHashSetTest) {
    // Insert 
    char * value1 = "value1";
    char * value2 = "value2";
    char * value3 = "value3";
    char * value4 = "value4";
    
    hashSetInsert(hash_set, strlen(value1), value1);
    hashSetInsert(hash_set, strlen(value2), value2);
    hashSetInsert(hash_set, strlen(value3), value3);
    hashSetInsert(hash_set, strlen(value4), value4);
   
    EXPECT_EQ(hash_set -> size, 4);
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value1), value1),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value2), value2),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value3), value3),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value4), value4),
        true
    );

    hash_set = resizeHashSet(hash_set, 20);

    EXPECT_EQ(hash_set -> capacity, 20);
    EXPECT_EQ(hash_set -> size, 4);
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value1), value1),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value2), value2),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value3), value3),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value4), value4),
        true
    );

    // Check that a capacity less than or equal to the current capacity results in a failure
    const char formatter1[] = "File: %s.\nOperation: resizeHashSet.\nMessage: %s\n";
    const char file1[] = "src/ccollections/set/hashset.c";
    const char message1[] = "The new capacity cannot less or equal to the existing capacity.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(resizeHashSet(hash_set, 20), expected_message1);

    free(expected_message1);

    // We delete the hashSet, without running into null pointer accesses
    deleteHashSet(&hash_set, nullptr);

    const char formatter2[] = "File: %s.\nOperation: resizeHashSet.\nMessage: %s\n";
    const char file2[] = "src/ccollections/set/hashset.c";
    const char message2[] = "The parameter <set> cannot be NULL.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(resizeHashSet(hash_set, 20), expected_message2);

    free(expected_message2);
}

// isHashSetEmpty
TEST_F(HashSetTest, isHashSetEmptyTest) {
    // No elements have been added to the hash set, it should be empty
    EXPECT_EQ(isHashSetEmpty(hash_set), true);

    // We delete the hash set, without running into null pointer accesses
    deleteHashSet(&hash_set, nullptr);

    const char formatter[] = "File: %s.\nOperation: isHashSetEmpty.\nMessage: %s\n";
    const char file[] = "src/ccollections/set/hashset.c";
    const char message[] = "The parameter <set> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(isHashSetEmpty(hash_set), expected_message);

    free(expected_message);
}

// hashSetInsert
TEST_F(HashSetTest, hashSetInsertTest) {
    // Insert 
    char * value1 = "value1";
    char * value2 = "value2";
    char * value3 = "value3";
    char * value4 = "value4";
    
    hashSetInsert(hash_set, strlen(value1), value1);
    hashSetInsert(hash_set, strlen(value2), value2);
    hashSetInsert(hash_set, strlen(value3), value3);
    hashSetInsert(hash_set, strlen(value4), value4);
    EXPECT_EQ(hash_set -> size, 4);

    uint64_t true_hash1 = siphash24(value1, strlen(value1), hash_set -> hash_key);
    EXPECT_STREQ(
        (char *) hash_set -> items[true_hash1 % hash_set -> capacity] -> value,
        "value1"
    );

    uint64_t true_hash2 = siphash24(value2, strlen(value2), hash_set -> hash_key);
    EXPECT_STREQ(
        (char *) hash_set -> items[true_hash2 % hash_set -> capacity] -> value,
        "value2"
    );

    uint64_t true_hash3 = siphash24(value3, strlen(value3), hash_set -> hash_key);
    EXPECT_STREQ(
        (char *) hash_set -> items[true_hash3 % hash_set -> capacity] -> value,
        "value3"
    );

    uint64_t true_hash4 = siphash24(value4, strlen(value4), hash_set -> hash_key);
    EXPECT_STREQ(
        (char *) hash_set -> items[true_hash4 % hash_set -> capacity] -> value,
        "value4"
    );

    // We delete the hashSet, we should not be able to insert into it
    deleteHashSet(&hash_set, nullptr);

    const char formatter[] = "File: %s.\nOperation: hashSetInsert.\nMessage: %s\n";
    const char file[] = "src/ccollections/set/hashset.c";
    const char message[] = "The parameter <set> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(
        hashSetInsert(hash_set, strlen(value1), value1),
        expected_message
    );

    free(expected_message);
}

// hashSetContains
TEST_F(HashSetTest, hashSetContainsTest) {
    char * value1 = "value1";
    char * value2 = "value2";
    char * value3 = "value3";
    char * value4 = "value4";
    
    hashSetInsert(hash_set, strlen(value1), value1);
    hashSetInsert(hash_set, strlen(value2), value2);
    hashSetInsert(hash_set, strlen(value3), value3);

    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value1), value1),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value2), value2),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value3), value3),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value4), value4),
        false
    );

    // We delete the hashSet, we should not be able to check if elements exist in it
    deleteHashSet(&hash_set, nullptr);

    const char formatter[] = "File: %s.\nOperation: hashSetContains.\nMessage: %s\n";
    const char file[] = "src/ccollections/set/hashset.c";
    const char message[] = "The parameter <set> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(
        hashSetContains(hash_set, strlen(value1), value1),
        expected_message
    );

    free(expected_message);
}

// hashSetDelete
TEST_F(HashSetTest, hashSetDeleteTest) {
    // Insert a few values
    char * value1 = "value1";
    char * value2 = "value2";
    char * value3 = "value3";
    
    hashSetInsert(hash_set, strlen(value1), value1);
    hashSetInsert(hash_set, strlen(value2), value2);
    hashSetInsert(hash_set, strlen(value3), value3);
    EXPECT_EQ(hash_set -> size, 3);

    hashSetDelete(hash_set, strlen(value1), value1, NULL);
    EXPECT_EQ(hash_set -> size, 2);
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value1), value1),
        false
    );

    // We delete the hashSet, we should not be able to delete anything from it
    deleteHashSet(&hash_set, nullptr);

    const char formatter[] = "File: %s.\nOperation: hashSetDelete.\nMessage: %s\n";
    const char file[] = "src/ccollections/set/hashset.c";
    const char message[] = "The parameter <set> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(
        hashSetDelete(hash_set, strlen(value1), value1, NULL),
        expected_message
    );

    free(expected_message);
}

// ->atEnd
TEST_F(HashSetTest, hashSet_atEnd_Test) {
    // Insert a few values
    char * value1 = "value1";
    char * value2 = "value2";
    char * value3 = "value3";

    hashSetInsert(hash_set, strlen(value1), value1);
    hashSetInsert(hash_set, strlen(value2), value2);
    hashSetInsert(hash_set, strlen(value3), value3);

    EXPECT_EQ(hash_set -> collection.atEnd(&hash_set -> collection, 2), false);
    EXPECT_EQ(hash_set -> collection.atEnd(&hash_set -> collection, 3), true);
}

// ->get
TEST_F(HashSetTest, hashSet_get_Test) {
    // Insert a few values
    char * value1 = "value1";
    char * value2 = "value2";
    char * value3 = "value3";

    std::vector<char *> values = {"value1", "value2", "value3"};

    hashSetInsert(hash_set, strlen(value1), value1);
    hashSetInsert(hash_set, strlen(value2), value2);
    hashSetInsert(hash_set, strlen(value3), value3);

    struct HashSetItem * item = (struct HashSetItem *) hash_set -> collection.get(&hash_set -> collection, 2);
    ASSERT_THAT(values, ::testing::Contains(item -> value));

    // Cannot get from an out of bounds index
    const char formatter[] = "File: %s.\nOperation: _hashSetCollectionGet.\nMessage: %s\n";
    const char file[] = "src/ccollections/set/hashset.c";
    const char message[] = "The index is out of bounds.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(
        hash_set -> collection.get(&hash_set -> collection, 3),
        expected_message
    );

    free(expected_message);
}
