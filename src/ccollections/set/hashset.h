/*  This file is part of the CCollections library.
 * 
 *  Copyright (c) 2022- Ntwali B. Toussaint
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef CCOLLECTIONS_HASHSET_H
#define CCOLLECTIONS_HASHSET_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#include "../common/interfaces.h"

extern float set_growth_factor;


struct HashSetItem {
    void * value;
    unsigned value_len;
    uint64_t hash;
    struct HashSetItem * prev;
    struct HashSetItem * next;
};

struct HashSet {
    struct Collection collection;
    struct HashSetItem ** items;
    char hash_key[16];
    unsigned capacity;
    unsigned size;
    unsigned buckets_count;
};


/**
 * Initializes the set
 *
 * @return      the newly created set.
 */
struct HashSet * newHashSet(unsigned initial_capacity);


/**
 * Frees the memory occupied by the set.
 *
 * @param       set pointer to memory occupied by the set.
 */
void deleteHashSet(struct HashSet ** const set, CDeleter deleter);


/**
 * Resizes the given set to higher capacity.
 *
 * @param       set pointer to set to resize.
 * @param       new_capacity the new capacity of the set.
 *
 * @return      the newly resized set.
 */
struct HashSet * resizeHashSet(struct HashSet * const set, unsigned new_capacity);


/**
 * Check if the set is empty.
 *
 * @param       set pointer to the set which content to check.
 *
 * @return      true if the set is empty, false otherwise.
 */
bool isHashSetEmpty(struct HashSet const * const set);


/**
 * Check if the set is full.
 *
 * @param       set pointer to the set which content to check.
 *
 * @return      true if the set is full, false otherwise.
 */
bool isHashSetFull(struct HashSet const * const set);


/**
 * Inserts a value into the set.
 *
 * @param       set     pointer to set to append an element to.
 * @param       value_len the length of the value in bytes.
 * @param       value     the value to store into the set.
 *
 * @return      true if the value was inserted, false otherwise
 */
bool hashSetInsert(struct HashSet * const set, unsigned value_len, void * value);


/**
 * Check if the set contains the given value.
 *
 * @param       set     pointer to set to use.
 * @param       value   the value to look for.
 *
 * @return      true if the value is in the set, false otherwise.
 */
bool hashSetContains(struct HashSet const * const set, unsigned value_len, void * value);


/**
 * Removes the given value from the set.
 *
 * @param       set pointer to set to use.
 * @param       value the value to remove from the set.
 *
 * @return      true if the value was removed, false otherwise.
 */
bool hashSetDelete(struct HashSet * const set, unsigned value_len, void * value, CDeleter deleter);

#endif
