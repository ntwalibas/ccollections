#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

extern "C" {
    #include "map.h"
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
    EXPECT_DEATH(newHashMap(0), ::testing::HasSubstr("Initial hash map capacity cannot be zero."));
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
    const char * pair1[] = {"key1", "value1"};
    const char * pair2[] = {"key2", "value2"};
    const char * pair3[] = {"key3", "value3"};
    const char * pair4[] = {"key4", "value4"};
    
    hashMapInsert(hash_map, strlen(pair1[0]), pair1[0], const_cast<char *>(pair1[1]));
    hashMapInsert(hash_map, strlen(pair2[0]), pair2[0], const_cast<char *>(pair2[1]));
    hashMapInsert(hash_map, strlen(pair3[0]), pair3[0], const_cast<char *>(pair3[1]));
    hashMapInsert(hash_map, strlen(pair4[0]), pair4[0], const_cast<char *>(pair4[1]));
   
    EXPECT_EQ(hash_map -> size, 4);
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair1[0]), pair1[0]),
        "value1"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair2[0]), pair2[0]),
        "value2"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair3[0]), pair3[0]),
        "value3"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair4[0]), pair4[0]),
        "value4"
    );

    hash_map = resizeHashMap(hash_map, 20);

    EXPECT_EQ(hash_map -> capacity, 20);
    EXPECT_EQ(hash_map -> size, 4);
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair1[0]), pair1[0]),
        "value1"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair2[0]), pair2[0]),
        "value2"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair3[0]), pair3[0]),
        "value3"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair4[0]), pair4[0]),
        "value4"
    );

    // Check that a capacity less than or equal to the current capacity results in a failure
    EXPECT_DEATH(resizeHashMap(hash_map, 20), ::testing::HasSubstr("The new capacity cannot be less or equal to the existing capacity."));

    // We delete the hashMap, without running into null pointer accesses
    deleteHashMap(&hash_map, nullptr);
    EXPECT_DEATH(resizeHashMap(hash_map, 20), ::testing::HasSubstr("The parameter <map> cannot be NULL."));
}

// isHashMapEmpty
TEST_F(HashMapTest, isHashMapEmptyTest) {
    // No elements have been added to the hash map, it should be empty
    EXPECT_EQ(isHashMapEmpty(hash_map), true);

    // We delete the hash map, without running into null pointer accesses
    deleteHashMap(&hash_map, nullptr);
    EXPECT_DEATH(isHashMapEmpty(hash_map), testing::HasSubstr("The parameter <map> cannot be NULL."));
}

// hashMapInsert
TEST_F(HashMapTest, hashMapInsertTest) {
    // Insert 
    const char * pair1[] = {"key1", "value1"};
    const char * pair2[] = {"key2", "value2"};
    const char * pair3[] = {"key3", "value3"};
    const char * pair4[] = {"key4", "value4"};
    
    hashMapInsert(hash_map, strlen(pair1[0]), pair1[0], const_cast<char *>(pair1[1]));
    hashMapInsert(hash_map, strlen(pair2[0]), pair2[0], const_cast<char *>(pair2[1]));
    hashMapInsert(hash_map, strlen(pair3[0]), pair3[0], const_cast<char *>(pair3[1]));
    hashMapInsert(hash_map, strlen(pair4[0]), pair4[0], const_cast<char *>(pair4[1]));
    EXPECT_EQ(hash_map -> size, 4);

    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair1[0]), pair1[0]),
        "value1"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair2[0]), pair2[0]),
        "value2"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair3[0]), pair3[0]),
        "value3"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair4[0]), pair4[0]),
        "value4"
    );

    // We delete the hashMap, we should not be able to insert into it
    deleteHashMap(&hash_map, nullptr);
    EXPECT_DEATH(
        hashMapInsert(hash_map, strlen(pair1[0]), pair1[0], const_cast<char *>(pair1[1])),
        ::testing::HasSubstr("The parameter <map> cannot be NULL.")
    );
}

// hashMapGet
TEST_F(HashMapTest, hashMapGetTest) {
    const char * pair1[] = {"key1", "value1"};
    const char * pair2[] = {"key2", "value2"};
    const char * pair3[] = {"key3", "value3"};
    const char * pair4[] = {"key4", "value4"};
    
    hashMapInsert(hash_map, strlen(pair1[0]), pair1[0], const_cast<char *>(pair1[1]));
    hashMapInsert(hash_map, strlen(pair2[0]), pair2[0], const_cast<char *>(pair2[1]));
    hashMapInsert(hash_map, strlen(pair3[0]), pair3[0], const_cast<char *>(pair3[1]));
    hashMapInsert(hash_map, strlen(pair4[0]), pair4[0], const_cast<char *>(pair4[1]));

    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair1[0]), pair1[0]),
        "value1"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair2[0]), pair2[0]),
        "value2"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair3[0]), pair3[0]),
        "value3"
    );
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair4[0]), pair4[0]),
        "value4"
    );

    // We delete the hashMap, we should not be able to get elements out of it
    deleteHashMap(&hash_map, nullptr);
    EXPECT_DEATH(
        hashMapGet(hash_map, strlen(pair1[0]), pair1[0]),
        ::testing::HasSubstr("The parameter <map> cannot be NULL.")
    );
}

// hashMapSet
TEST_F(HashMapTest, hashMapSetTest) {
    // Insert a few values
    const char * pair1[] = {"key1", "value1"};
    const char * pair2[] = {"key2", "value2"};
    const char * pair3[] = {"key3", "value3"};
    const char * pair4[] = {"key4", "value4"};
    
    hashMapInsert(hash_map, strlen(pair1[0]), pair1[0], const_cast<char *>(pair1[1]));
    hashMapInsert(hash_map, strlen(pair2[0]), pair2[0], const_cast<char *>(pair2[1]));
    hashMapInsert(hash_map, strlen(pair3[0]), pair3[0], const_cast<char *>(pair3[1]));
    EXPECT_EQ(hash_map -> size, 3);

    // Validate that before setting a new value to an existing key, it has the expected vale
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair2[0]), pair2[0]),
        "value2"
    );

    // Set a new value for key = "key2" and verify that it was replaced
    const char * new_value = "new_value";
    hashMapSet(hash_map, strlen(pair2[0]), pair2[0], const_cast<char *>(new_value));
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair2[0]), pair2[0]),
        "new_value"
    );

    // Set a non-existing key and verify that it was inserted
    hashMapSet(hash_map, strlen(pair4[0]), pair4[0], const_cast<char *>(pair4[1]));
    EXPECT_STREQ(
        (const char *) hashMapGet(hash_map, strlen(pair4[0]), pair4[0]),
        "value4"
    );

    // We delete the hashMap, we should not be able to set a value for the given key into it
    deleteHashMap(&hash_map, nullptr);
    EXPECT_DEATH(
        hashMapSet(hash_map, strlen(pair1[0]), pair1[0], const_cast<char *>(pair1[1])),
        ::testing::HasSubstr("The parameter <map> cannot be NULL.")
    );
}

// hashMapDelete
TEST_F(HashMapTest, hashMapDeleteTest) {
    // Insert a few values
    const char * pair1[] = {"key1", "value1"};
    const char * pair2[] = {"key2", "value2"};
    const char * pair3[] = {"key3", "value3"};
    
    hashMapInsert(hash_map, strlen(pair1[0]), pair1[0], const_cast<char *>(pair1[1]));
    hashMapInsert(hash_map, strlen(pair2[0]), pair2[0], const_cast<char *>(pair2[1]));
    hashMapInsert(hash_map, strlen(pair3[0]), pair3[0], const_cast<char *>(pair3[1]));
    EXPECT_EQ(hash_map -> size, 3);

    hashMapDelete(hash_map, strlen(pair1[0]), pair1[0], nullptr);
    EXPECT_EQ(hash_map -> size, 2);
    EXPECT_EQ(
        hashMapGet(hash_map, strlen(pair1[0]), pair1[0]),
        nullptr
    );

    // We delete the hashMap, we should not be able to delete anything from it
    deleteHashMap(&hash_map, nullptr);
    EXPECT_DEATH(
        hashMapDelete(hash_map, strlen(pair1[0]), pair1[0], nullptr),
        ::testing::HasSubstr("The parameter <map> cannot be NULL.")
    );
}

// ->atEnd
TEST_F(HashMapTest, hashMap_atEnd_Test) {
    // Insert a few values
    const char * pair1[] = {"key1", "value1"};
    const char * pair2[] = {"key2", "value2"};
    const char * pair3[] = {"key3", "value3"};

    hashMapInsert(hash_map, strlen(pair1[0]), pair1[0], const_cast<char *>(pair1[1]));
    hashMapInsert(hash_map, strlen(pair2[0]), pair2[0], const_cast<char *>(pair2[1]));
    hashMapInsert(hash_map, strlen(pair3[0]), pair3[0], const_cast<char *>(pair3[1]));

    EXPECT_EQ(hash_map -> collection.atEnd(&hash_map -> collection, 2), false);
    EXPECT_EQ(hash_map -> collection.atEnd(&hash_map -> collection, 3), true);
}

// ->get
TEST_F(HashMapTest, hashMap_get_Test) {
    // Insert a few values
    const char * pair1[] = {"key1", "value1"};
    const char * pair2[] = {"key2", "value2"};
    const char * pair3[] = {"key3", "value3"};

    std::vector<const char *> keys = {"key1", "key2", "key3"};
    std::vector<const char *> values = {"value1", "value2", "value3"};

    hashMapInsert(hash_map, strlen(pair1[0]), pair1[0], const_cast<char *>(pair1[1]));
    hashMapInsert(hash_map, strlen(pair2[0]), pair2[0], const_cast<char *>(pair2[1]));
    hashMapInsert(hash_map, strlen(pair3[0]), pair3[0], const_cast<char *>(pair3[1]));

    struct HashMapItem * item = (struct HashMapItem *) hash_map -> collection.get(&hash_map -> collection, 2);
    ASSERT_THAT(keys, ::testing::Contains(item -> key));
    ASSERT_THAT(values, ::testing::Contains(item -> value));

    // Cannot get from an out of bounds index
    EXPECT_DEATH(
        hash_map -> collection.get(&hash_map -> collection, 3),
        ::testing::HasSubstr("The index is out of bounds.")
    );
}
