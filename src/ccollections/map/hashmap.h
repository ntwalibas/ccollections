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

#ifndef CCOLLECTIONS_MAP_H
#define CCOLLECTIONS_MAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#include "../interfaces/interfaces.h"

extern float map_growth_factor;


struct HashMapItem {
    void const * key;
    void * value;
    unsigned key_len;
    uint64_t hash;
    struct HashMapItem * prev;
    struct HashMapItem * next;
};

struct HashMap {
    struct Collection collection;
    struct HashMapItem ** items;
    char hash_key[16];
    unsigned capacity;
    unsigned size;
    unsigned buckets_count;
};


/**
 * Initializes the map
 *
 * @return      the newly created map.
 */
struct HashMap * newHashMap(unsigned initial_capacity);


/**
 * Frees the memory occupied by the map.
 *
 * @param       map pointer to memory occupied by the map.
 */
void deleteHashMap(struct HashMap ** const map, CDeleter deleter);


/**
 * Resizes the given map to higher capacity.
 *
 * @param       map pointer to map to resize.
 * @param       new_capacity the new capacity of the map.
 *
 * @return      the newly resized map.
 */
struct HashMap * resizeHashMap(struct HashMap * const map, unsigned new_capacity);


/**
 * Check if the map is empty.
 *
 * @param       map pointer to the map which content to check.
 *
 * @return      true if the map is empty, false otherwise.
 */
bool isHashMapEmpty(struct HashMap const * const map);


/**
 * Check if the map is full.
 *
 * @param       map pointer to the map which content to check.
 *
 * @return      true if the map is full, false otherwise.
 */
bool isHashMapFull(struct HashMap const * const map);


/**
 * Inserts a key-value pair into the map.
 *
 * @param       map     pointer to map to append an element to.
 * @param       key_len the length of the key in bytes.
 * @param       key     the key to associate to the value.
 * @param       value   pointer to the value to add to the map.
 *
 * @return      true if the key-value pair was inserted, false otherwise
 */
bool hashMapInsert(struct HashMap * const map, unsigned key_len, void const * key, void * value);


/**
 * Get the value for the specified key.
 *
 * @param       map pointer to map to use.
 * @param       key the index at which to look.
 *
 * @return      the value associated to the given key.
 */
void * hashMapGet(struct HashMap const * const map, unsigned key_len, void * key);


/*
 * Changes the value associated to the given key, if it exists.
 * If the key doesn't exist, the new key-value pair will be inserted.
 *
 * @param       map     pointer to map to append an element to.
 * @param       key_len the length of the key in bytes.
 * @param       key     the key to associate to the value.
 * @param       value   pointer to the value to add to the map.
 *
 * @return      the value that was replaced if done, otherwise NULL.
 */
void * hashMapSet(struct HashMap * const map, unsigned key_len, void const * key, void * value);

#endif
