#include <gtest/gtest.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
    #include "src/ccollections/map/hashmap.h"
    #include "src/ccollections/map/siphash.h"
}

class HashMapTest: public ::testing::Test {
    protected:
        void SetUp() override {
            hash_map = newHashMap(10);
        }

        void TearDown() override {
            deleteHashMap(&hash_map, nullptr);
        }
    
        struct HashMap * hash_map;
};

// newHashMap
TEST_F(HashMapTest, newHashMapTest) {
    // Expect that a new hashMap was created
    EXPECT_NE(hash_map, nullptr);

    // Expect that the hashMap capacity is equal to 10
    EXPECT_EQ(hash_map -> capacity, 10);

    // Check that a different hashMap with size 0 fails to be created
    const char formatter[] = "File: %s.\nOperation: newHashMap.\nMessage: %s\n";
    const char file[] = "src/ccollections/map/hashmap.c";
    const char message[] = "Initial hash map capacity cannot be zero.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(newHashMap(0), expected_message);

    free(expected_message);
}

// deleteHashMap
TEST_F(HashMapTest, deleteHashMapTest) {
    deleteHashMap(&hash_map, nullptr);

    // Make sure the hashMap is freed upon calling deleteHashMap
    EXPECT_EQ(hash_map, nullptr);
}

// resizeHashMap
TEST_F(HashMapTest, resizeHashMapTest) {
    // Insert 
    char * pair1[] = {"key1", "value1"};
    char * pair2[] = {"key2", "value2"};
    char * pair3[] = {"key3", "value3"};
    char * pair4[] = {"key4", "value4"};
    
    hashMapInsert(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0], pair1[1]);
    hashMapInsert(hash_map, sizeof(pair2[0]) / sizeof(char), pair2[0], pair2[1]);
    hashMapInsert(hash_map, sizeof(pair3[0]) / sizeof(char), pair3[0], pair3[1]);
    hashMapInsert(hash_map, sizeof(pair4[0]) / sizeof(char), pair4[0], pair4[1]);
   
    EXPECT_EQ(hash_map -> size, 4);
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0]),
        "value1"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair2[0]) / sizeof(char), pair2[0]),
        "value2"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair3[0]) / sizeof(char), pair3[0]),
        "value3"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair4[0]) / sizeof(char), pair4[0]),
        "value4"
    );

    hash_map = resizeHashMap(hash_map, 20);

    EXPECT_EQ(hash_map -> capacity, 20);
    EXPECT_EQ(hash_map -> size, 4);
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0]),
        "value1"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair2[0]) / sizeof(char), pair2[0]),
        "value2"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair3[0]) / sizeof(char), pair3[0]),
        "value3"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair4[0]) / sizeof(char), pair4[0]),
        "value4"
    );

    // Check that a capacity less than or equal to the current capacity results in a failure
    const char formatter1[] = "File: %s.\nOperation: resizeHashMap.\nMessage: %s\n";
    const char file1[] = "src/ccollections/map/hashmap.c";
    const char message1[] = "The new capacity cannot less or equal to the existing capacity.";
    
    int size1 = snprintf(NULL, 0, formatter1, file1, message1);
    char * expected_message1 = (char *) malloc((size1 + 1) * sizeof(char));
    snprintf(expected_message1, size1 + 1, formatter1, file1, message1);

    EXPECT_DEATH(resizeHashMap(hash_map, 20), expected_message1);

    free(expected_message1);

    // We delete the hashMap, without running into null pointer accesses
    deleteHashMap(&hash_map, nullptr);

    const char formatter2[] = "File: %s.\nOperation: resizeHashMap.\nMessage: %s\n";
    const char file2[] = "src/ccollections/map/hashmap.c";
    const char message2[] = "The parameter <map> cannot be NULL.";
    
    int size2 = snprintf(NULL, 0, formatter2, file2, message2);
    char * expected_message2 = (char *) malloc((size2 + 1) * sizeof(char));
    snprintf(expected_message2, size2 + 1, formatter2, file2, message2);

    EXPECT_DEATH(resizeHashMap(hash_map, 20), expected_message2);

    free(expected_message2);
}

// isHashMapEmpty
TEST_F(HashMapTest, isHashMapEmptyTest) {
    // No elements have been added to the hash map, it should be empty
    EXPECT_EQ(isHashMapEmpty(hash_map), true);

    // We delete the hash map, without running into null pointer accesses
    deleteHashMap(&hash_map, nullptr);

    const char formatter[] = "File: %s.\nOperation: isHashMapEmpty.\nMessage: %s\n";
    const char file[] = "src/ccollections/map/hashmap.c";
    const char message[] = "The parameter <map> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(isHashMapEmpty(hash_map), expected_message);

    free(expected_message);
}

// hashMapInsert
TEST_F(HashMapTest, hashMapInsertTest) {
    // Insert 
    char * pair1[] = {"key1", "value1"};
    char * pair2[] = {"key2", "value2"};
    char * pair3[] = {"key3", "value3"};
    char * pair4[] = {"key4", "value4"};
    
    hashMapInsert(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0], pair1[1]);
    hashMapInsert(hash_map, sizeof(pair2[0]) / sizeof(char), pair2[0], pair2[1]);
    hashMapInsert(hash_map, sizeof(pair3[0]) / sizeof(char), pair3[0], pair3[1]);
    hashMapInsert(hash_map, sizeof(pair4[0]) / sizeof(char), pair4[0], pair4[1]);
    EXPECT_EQ(hash_map -> size, 4);

    uint64_t true_hash1 = siphash24(
        (char const *) pair1[0], sizeof(pair1[0]) / sizeof(char),
        hash_map -> hash_key
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash1 % hash_map -> capacity] -> key,
        "key1"
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash1 % hash_map -> capacity] -> value,
        "value1"
    );

    uint64_t true_hash2 = siphash24(
        (char const *) pair2[0], sizeof(pair2[0]) / sizeof(char),
        hash_map -> hash_key
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash2 % hash_map -> capacity] -> key,
        "key2"
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash2 % hash_map -> capacity] -> value,
        "value2"
    );

    uint64_t true_hash3 = siphash24(
        (char const *) pair3[0], sizeof(pair3[0]) / sizeof(char),
        hash_map -> hash_key
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash3 % hash_map -> capacity] -> key,
        "key3"
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash3 % hash_map -> capacity] -> value,
        "value3"
    );

    uint64_t true_hash4 = siphash24(
        (char const *) pair4[0], sizeof(pair4[0]) / sizeof(char),
        hash_map -> hash_key
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash4 % hash_map -> capacity] -> key,
        "key4"
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash4 % hash_map -> capacity] -> value,
        "value4"
    );

    // We delete the hashMap, we should not be able to insert into it
    deleteHashMap(&hash_map, nullptr);

    const char formatter[] = "File: %s.\nOperation: hashMapInsert.\nMessage: %s\n";
    const char file[] = "src/ccollections/map/hashmap.c";
    const char message[] = "The parameter <map> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(
        hashMapInsert(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0], pair1[1]),
        expected_message
    );

    free(expected_message);
}

// hashMapGet
TEST_F(HashMapTest, hashMapGetTest) {
    char * pair1[] = {"key1", "value1"};
    char * pair2[] = {"key2", "value2"};
    char * pair3[] = {"key3", "value3"};
    char * pair4[] = {"key4", "value4"};
    
    hashMapInsert(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0], pair1[1]);
    hashMapInsert(hash_map, sizeof(pair2[0]) / sizeof(char), pair2[0], pair2[1]);
    hashMapInsert(hash_map, sizeof(pair3[0]) / sizeof(char), pair3[0], pair3[1]);
    hashMapInsert(hash_map, sizeof(pair4[0]) / sizeof(char), pair4[0], pair4[1]);

    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0]),
        "value1"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair2[0]) / sizeof(char), pair2[0]),
        "value2"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair3[0]) / sizeof(char), pair3[0]),
        "value3"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, sizeof(pair4[0]) / sizeof(char), pair4[0]),
        "value4"
    );

    // We delete the hashMap, we should not be able to get elements out of it
    deleteHashMap(&hash_map, nullptr);

    const char formatter[] = "File: %s.\nOperation: hashMapGet.\nMessage: %s\n";
    const char file[] = "src/ccollections/map/hashmap.c";
    const char message[] = "The parameter <map> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(
        hashMapGet(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0]),
        expected_message
    );

    free(expected_message);
}

// hashMapSet
TEST_F(HashMapTest, hashMapSetTest) {
    // Insert a few values
    char * pair1[] = {"key1", "value1"};
    char * pair2[] = {"key2", "value2"};
    char * pair3[] = {"key3", "value3"};
    char * pair4[] = {"key4", "value4"};
    
    hashMapInsert(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0], pair1[1]);
    hashMapInsert(hash_map, sizeof(pair2[0]) / sizeof(char), pair2[0], pair2[1]);
    hashMapInsert(hash_map, sizeof(pair3[0]) / sizeof(char), pair3[0], pair3[1]);
    EXPECT_EQ(hash_map -> size, 3);

    uint64_t true_hash2 = siphash24(
        (char const *) pair2[0], sizeof(pair2[0]) / sizeof(char),
        hash_map -> hash_key
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash2 % hash_map -> capacity] -> key,
        "key2"
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash2 % hash_map -> capacity] -> value,
        "value2"
    );

    // Set a new value for key = "key2" and verify that it was replaced
    char * new_value = "new_value";
    hashMapSet(hash_map, sizeof(pair2[0]) / sizeof(char), pair2[0], new_value);
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash2 % hash_map -> capacity] -> key,
        "key2"
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash2 % hash_map -> capacity] -> value,
        "new_value"
    );

    // Set a non-existing key and verify that it was inserted
    hashMapSet(hash_map, sizeof(pair4[0]) / sizeof(char), pair4[0], pair4[1]);
    uint64_t true_hash4 = siphash24(
        (char const *) pair4[0], sizeof(pair4[0]) / sizeof(char),
        hash_map -> hash_key
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash4 % hash_map -> capacity] -> key,
        "key4"
    );
    EXPECT_STREQ(
        (const char *) hash_map -> items[true_hash4 % hash_map -> capacity] -> value,
        "value4"
    );

    // We delete the hashMap, we should not be able to set a value for the given key into it
    deleteHashMap(&hash_map, nullptr);

    const char formatter[] = "File: %s.\nOperation: hashMapSet.\nMessage: %s\n";
    const char file[] = "src/ccollections/map/hashmap.c";
    const char message[] = "The parameter <map> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(
        hashMapSet(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0], pair1[1]),
        expected_message
    );

    free(expected_message);
}

// hashMapDelete
TEST_F(HashMapTest, hashMapDeleteTest) {
    // Insert a few values
    char * pair1[] = {"key1", "value1"};
    char * pair2[] = {"key2", "value2"};
    char * pair3[] = {"key3", "value3"};
    
    hashMapInsert(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0], pair1[1]);
    hashMapInsert(hash_map, sizeof(pair2[0]) / sizeof(char), pair2[0], pair2[1]);
    hashMapInsert(hash_map, sizeof(pair3[0]) / sizeof(char), pair3[0], pair3[1]);
    EXPECT_EQ(hash_map -> size, 3);

    hashMapDelete(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0], NULL);
    EXPECT_EQ(hash_map -> size, 2);
    EXPECT_EQ(
        hashMapGet(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0]),
        nullptr
    );

    // We delete the hashMap, we should not be able to delete anything from it
    deleteHashMap(&hash_map, nullptr);

    const char formatter[] = "File: %s.\nOperation: hashMapDelete.\nMessage: %s\n";
    const char file[] = "src/ccollections/map/hashmap.c";
    const char message[] = "The parameter <map> cannot be NULL.";
    
    int size = snprintf(NULL, 0, formatter, file, message);
    char * expected_message = (char *) malloc((size + 1) * sizeof(char));
    snprintf(expected_message, size + 1, formatter, file, message);

    EXPECT_DEATH(
        hashMapDelete(hash_map, sizeof(pair1[0]) / sizeof(char), pair1[0], NULL),
        expected_message
    );

    free(expected_message);
}

