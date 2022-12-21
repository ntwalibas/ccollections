/*  This file is part of the CCollections library.
 * 
 *  Copyright (c) 2022- Ntwali B. Toussaint
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *`
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "siphash.h"
#include "set.h"

static void * _hashSetCollectionGet(struct Collection * const collection, unsigned index);
static bool _hashSetCollectionAtEnd(struct Collection const * const collection, unsigned index);

static void * createItem(unsigned value_len, void * value, uint64_t hash, struct HashSetItem * prev, struct HashSetItem * next);
static void deleteItem(struct HashSetItem * item, CDeleter deleter);

float hash_set_growth_factor = 1.75;


/**
 * Initializes the set
 *
 * @return      the newly created set.
 */
struct HashSet * newHashSet(unsigned initial_capacity) {
    alt_assert(initial_capacity != 0, "Initial hash set capacity cannot be zero.");

    struct HashSet * set = malloc(sizeof *set);
    if (set == NULL)
       return NULL;

    set -> items = calloc(initial_capacity, sizeof *set -> items);
    if (set -> items == NULL) {
        free(set);
        return NULL;
    }

    struct Collection collection = {
        .get = _hashSetCollectionGet,
        .set = NULL,
        .atEnd = _hashSetCollectionAtEnd,
    };

    set -> collection = collection;
    // At the moment we fix the hash key but in the future we will randomize it
    char hash_key[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
    memcpy(set -> hash_key, hash_key, sizeof(set -> hash_key));
    set -> capacity = initial_capacity;
    set -> size = 0;
    set -> buckets_count = 0;

    return set;
}


/**
 * Frees the memory occupied by the set.
 *
 * @param       set pointer to memory occupied by the set.
 */
void deleteHashSet(struct HashSet ** const set, CDeleter deleter) {
    if (set == NULL)
        return;

    if (* set == NULL)
        return;
    
    for (unsigned i = 0; i < (* set) -> capacity; i++) {
        struct HashSetItem * item = (* set) -> items[i];
        if (item == NULL)
            continue;

        struct HashSetItem * next = NULL;
        do {
            next = item -> next;
            deleteItem(item, deleter);
            item = next;
        } while (next != NULL);
    }

    free((* set) -> items);
    free(* set);
    * set = NULL;
}


/**
 * Resizes the given set to higher capacity.
 *
 * @param       set pointer to set to resize.
 * @param       new_capacity the new capacity of the set.
 *
 * @return      the newly resized set.
 */
struct HashSet * resizeHashSet(struct HashSet * const set, unsigned new_capacity) {
    alt_assert(set != NULL, "The parameter <set> cannot be NULL.");
    alt_assert(new_capacity > set -> capacity, "The new capacity cannot less or equal to the existing capacity.");

    struct HashSetItem ** items = calloc(new_capacity, sizeof *items);
    if (items == NULL)
        return NULL;

    for (unsigned i = 0; i < set -> capacity; i++) {
        struct HashSetItem * item = set -> items[i];
        if (item == NULL)
            continue;
        
        struct HashSetItem * next_item = NULL;
        do {
            next_item = item -> next;

            uint64_t hash = siphash24((char const *) item -> value, item -> value_len, set -> hash_key);
            uint64_t hash_value = hash % new_capacity;
            item -> hash = hash;
            if (items[hash_value] != NULL)
                items[hash_value] -> prev = item;
            item -> next = items[hash_value];
            items[hash_value] = item;

            item = next_item;
        } while (next_item != NULL);
    }

    free(set -> items);
    set -> items = items;
    set -> capacity = new_capacity;

    return set;
}


/**
 * Check if the set is empty.
 *
 * @param       set pointer to the set which content to check.
 *
 * @return      true if the set is empty, false otherwise.
 */
bool isHashSetEmpty(struct HashSet const * const set) {
    alt_assert(set != NULL, "The parameter <set> cannot be NULL.");

    return set -> size == 0;
}


/**
 * Check if the set is full.
 *
 * @param       set pointer to the set which content to check.
 *
 * @return      true if the set is full, false otherwise.
 */
bool isHashSetFull(struct HashSet const * const set) {
    alt_assert(set != NULL, "The parameter <set> cannot be NULL.");

    return set -> size == set -> capacity;
}


/**
 * Inserts a value into the set.
 *
 * @param       set         pointer to set to append an element to.
 * @param       value_len   the length of the value in bytes.
 * @param       value       pointer to the value to add to the set.
 */
bool hashSetInsert(struct HashSet * const set, unsigned value_len, void * value) {
    alt_assert(set != NULL, "The parameter <set> cannot be NULL.");
    alt_assert(value_len != 0, "The size of the value (via value_len) cannot be zero.");

    // If the load factor exceeds 0.69, we resize the set
    float load_factor = (float)set -> buckets_count / (float)set -> capacity;
    // Maximum load factor pulled from: https://stackoverflow.com/a/31401836
    if (load_factor > 0.693) {
        int new_capacity = set -> capacity * hash_set_growth_factor;
        if (resizeHashSet(set, new_capacity) == NULL)
            return false;
    }

    uint64_t hash = siphash24((char const *) value, value_len, set -> hash_key);
    uint64_t hash_value = hash % set -> capacity;
    struct HashSetItem * existing_item = set -> items[hash_value];

    // If the value already exists in the set, no need to add it again
    if (existing_item != NULL && existing_item -> hash == hash)
        return true;
        
    // Incremement the number of buckets so we can calculate the load factor
    if (existing_item == NULL)
        set -> buckets_count += 1;

    struct HashSetItem * item = createItem(value_len, value, hash, NULL, existing_item);
    if (existing_item != NULL)
        existing_item -> prev = item;
    set -> items[hash_value] = item;
    set -> size++;

    return true;
}


/**
 * Check if the set contains the given value.
 *
 * @param       set     pointer to set to use.
 * @param       value   the value to look for.
 *
 * @return      true if the value is in the set, false otherwise.
 */
bool hashSetContains(struct HashSet const * const set, unsigned value_len, void * value) {
    alt_assert(set != NULL, "The parameter <set> cannot be NULL.");

    if (set -> size == 0)
        return false;

    uint64_t hash = siphash24((char const *) value, value_len, set -> hash_key);
    uint64_t hash_value = hash % set -> capacity;
    struct HashSetItem * item = set -> items[hash_value];

    if (item == NULL)
        return false;

    bool found = false;
    while (item != NULL) {
        if (item -> hash == hash) {
            found = true;
            break;
        }

        item = item -> next;
    }

    return found;
}


/**
 * Removes the given value from the set.
 *
 * @param       set pointer to set to use.
 * @param       value the value to remove from the set.
 *
 * @return      true if the value was removed, false otherwise.
 */
bool hashSetDelete(struct HashSet * const set, unsigned value_len, void * value, CDeleter deleter) {
    alt_assert(set != NULL, "The parameter <set> cannot be NULL.");
    alt_assert(value_len != 0, "The size of the value (via value_len) cannot be zero.");

    uint64_t hash = siphash24((char const *) value, value_len, set -> hash_key);
    uint64_t hash_value = hash % set -> capacity;

    struct HashSetItem * existing_item = set -> items[hash_value];
    if (existing_item == NULL)
        return false;
    
    do {
        if (existing_item -> hash == hash) {
            struct HashSetItem * prev = existing_item -> prev;
            struct HashSetItem * next = existing_item -> next;
            if (prev != NULL) {
                prev -> next = next;
                if (prev -> prev == NULL)
                    set -> items[hash_value] = prev;
            }

            if (next != NULL) {
                next -> prev = prev;
                if (prev == NULL)
                    set -> items[hash_value] = next;
            }

            if (prev == NULL && next == NULL) {
                set -> items[hash_value] = NULL;
                set -> buckets_count--;
            }

            deleteItem(existing_item, deleter);

            set -> size--;
            return true;
        }

        existing_item = existing_item -> next;
    } while(existing_item != NULL);

    return false;
}


static void * _hashSetCollectionGet(struct Collection * const collection, unsigned index) {
    alt_assert(collection != NULL, "The parameter <collection> cannot be NULL.");

    struct HashSet const * const set = (struct HashSet const * const) collection;
    
    alt_assert(set -> size > 0, "The hash set is empty, cannot get items.");
    alt_assert(index < set -> size, "The index is out of bounds.");

    unsigned shadow_index = 0;
    for (int i = 0; i < set -> capacity; i++) {
        struct HashSetItem * item = set -> items[i];
        if (item == NULL)
            continue;

        while (item != NULL) {
            if (shadow_index == index)
                return item;

            shadow_index++;
            item = item -> next;
        }
    }

    return NULL;
}


static bool _hashSetCollectionAtEnd(struct Collection const * const collection, unsigned index) {
    alt_assert(collection != NULL, "The parameter <collection> cannot be NULL.");
    
    struct HashSet const * const set = (struct HashSet const * const) collection;

    return index >= set -> size;
}


static void * createItem(unsigned value_len, void * value, uint64_t hash, struct HashSetItem * prev, struct HashSetItem * next) {
    struct HashSetItem * item = malloc(sizeof *item);
    if (item == NULL)
        return NULL;
    
    item -> value = value;
    item -> value_len = value_len;
    item -> hash = hash;
    item -> prev = prev;
    item -> next = next;

    return item;
}

static void deleteItem(struct HashSetItem * item, CDeleter deleter) {
    if (deleter != NULL)
        deleter(item -> value);

    free(item);
}
