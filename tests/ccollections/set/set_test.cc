#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

extern "C" {
    #include "set.h"
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
    EXPECT_DEATH(newHashSet(0), ::testing::HasSubstr("Initial hash set capacity cannot be zero."));
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
    const char * value1 = "value1";
    const char * value2 = "value2";
    const char * value3 = "value3";
    const char * value4 = "value4";
    
    hashSetInsert(hash_set, strlen(value1), const_cast<char *>(value1));
    hashSetInsert(hash_set, strlen(value2), const_cast<char *>(value2));
    hashSetInsert(hash_set, strlen(value3), const_cast<char *>(value3));
    hashSetInsert(hash_set, strlen(value4), const_cast<char *>(value4));
   
    EXPECT_EQ(hash_set -> size, 4);
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value1), const_cast<char *>(value1)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value2), const_cast<char *>(value2)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value3), const_cast<char *>(value3)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value4), const_cast<char *>(value4)),
        true
    );

    hash_set = resizeHashSet(hash_set, 20);

    EXPECT_EQ(hash_set -> capacity, 20);
    EXPECT_EQ(hash_set -> size, 4);
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value1), const_cast<char *>(value1)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value2), const_cast<char *>(value2)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value3), const_cast<char *>(value3)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value4), const_cast<char *>(value4)),
        true
    );

    // Check that a capacity less than or equal to the current capacity results in a failure
    EXPECT_DEATH(resizeHashSet(hash_set, 20), ::testing::HasSubstr("The new capacity cannot less or equal to the existing capacity."));

    // We delete the hashSet, without running into null pointer accesses
    deleteHashSet(&hash_set, nullptr);
    EXPECT_DEATH(resizeHashSet(hash_set, 20), ::testing::HasSubstr("The parameter <set> cannot be NULL."));
}

// isHashSetEmpty
TEST_F(HashSetTest, isHashSetEmptyTest) {
    // No elements have been added to the hash set, it should be empty
    EXPECT_EQ(isHashSetEmpty(hash_set), true);

    // We delete the hash set, without running into null pointer accesses
    deleteHashSet(&hash_set, nullptr);
    EXPECT_DEATH(isHashSetEmpty(hash_set), ::testing::HasSubstr("The parameter <set> cannot be NULL."));
}

// hashSetInsert
TEST_F(HashSetTest, hashSetInsertTest) {
    // Insert 
    const char * value1 = "value1";
    const char * value2 = "value2";
    const char * value3 = "value3";
    const char * value4 = "value4";
    
    hashSetInsert(hash_set, strlen(value1), const_cast<char *>(value1));
    hashSetInsert(hash_set, strlen(value2), const_cast<char *>(value2));
    hashSetInsert(hash_set, strlen(value3), const_cast<char *>(value3));
    hashSetInsert(hash_set, strlen(value4), const_cast<char *>(value4));
    EXPECT_EQ(hash_set -> size, 4);

    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value1), const_cast<char *>(value1)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value2), const_cast<char *>(value2)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value3), const_cast<char *>(value3)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value4), const_cast<char *>(value4)),
        true
    );

    // We delete the hashSet, we should not be able to insert into it
    deleteHashSet(&hash_set, nullptr);
    EXPECT_DEATH(
        hashSetInsert(hash_set, strlen(value1), const_cast<char *>(value1)),
        ::testing::HasSubstr("The parameter <set> cannot be NULL.")
    );
}

// hashSetContains
TEST_F(HashSetTest, hashSetContainsTest) {
    const char * value1 = "value1";
    const char * value2 = "value2";
    const char * value3 = "value3";
    const char * value4 = "value4";
    
    hashSetInsert(hash_set, strlen(value1), const_cast<char *>(value1));
    hashSetInsert(hash_set, strlen(value2), const_cast<char *>(value2));
    hashSetInsert(hash_set, strlen(value3), const_cast<char *>(value3));

    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value1), const_cast<char *>(value1)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value2), const_cast<char *>(value2)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value3), const_cast<char *>(value3)),
        true
    );
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value4), const_cast<char *>(value4)),
        false
    );

    // We delete the hashSet, we should not be able to check if elements exist in it
    deleteHashSet(&hash_set, nullptr);
    EXPECT_DEATH(
        hashSetContains(hash_set, strlen(value1), const_cast<char *>(value1)),
        ::testing::HasSubstr("The parameter <set> cannot be NULL.")
    );
}

// hashSetDelete
TEST_F(HashSetTest, hashSetDeleteTest) {
    // Insert a few values
    const char * value1 = "value1";
    const char * value2 = "value2";
    const char * value3 = "value3";
    
    hashSetInsert(hash_set, strlen(value1), const_cast<char *>(value1));
    hashSetInsert(hash_set, strlen(value2), const_cast<char *>(value2));
    hashSetInsert(hash_set, strlen(value3), const_cast<char *>(value3));
    EXPECT_EQ(hash_set -> size, 3);

    hashSetDelete(hash_set, strlen(value1), const_cast<char *>(value1), NULL);
    EXPECT_EQ(hash_set -> size, 2);
    EXPECT_EQ(
        hashSetContains(hash_set, strlen(value1), const_cast<char *>(value1)),
        false
    );

    // We delete the hashSet, we should not be able to delete anything from it
    deleteHashSet(&hash_set, nullptr);
    EXPECT_DEATH(
        hashSetDelete(hash_set, strlen(value1), const_cast<char *>(value1), NULL),
        ::testing::HasSubstr("The parameter <set> cannot be NULL.")
    );
}

// ->atEnd
TEST_F(HashSetTest, hashSet_atEnd_Test) {
    // Insert a few values
    const char * value1 = "value1";
    const char * value2 = "value2";
    const char * value3 = "value3";

    hashSetInsert(hash_set, strlen(value1), const_cast<char *>(value1));
    hashSetInsert(hash_set, strlen(value2), const_cast<char *>(value2));
    hashSetInsert(hash_set, strlen(value3), const_cast<char *>(value3));

    EXPECT_EQ(hash_set -> collection.atEnd(&hash_set -> collection, 2), false);
    EXPECT_EQ(hash_set -> collection.atEnd(&hash_set -> collection, 3), true);
}

// ->get
TEST_F(HashSetTest, hashSet_get_Test) {
    // Insert a few values
    const char * value1 = "value1";
    const char * value2 = "value2";
    const char * value3 = "value3";

    std::vector<const char *> values = {"value1", "value2", "value3"};

    hashSetInsert(hash_set, strlen(value1), const_cast<char *>(value1));
    hashSetInsert(hash_set, strlen(value2), const_cast<char *>(value2));
    hashSetInsert(hash_set, strlen(value3), const_cast<char *>(value3));

    struct HashSetItem * item = (struct HashSetItem *) hash_set -> collection.get(&hash_set -> collection, 2);
    ASSERT_THAT(values, ::testing::Contains(item -> value));

    // Cannot get from an out of bounds index
    EXPECT_DEATH(
        hash_set -> collection.get(&hash_set -> collection, 3),
        ::testing::HasSubstr("The index is out of bounds.")
    );
}
