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

#include "hashmap.h"
#include "siphash.h"

static void * _hashMapCollectionGet(struct Collection const * const collection, unsigned index);
static bool _hashMapCollectionAtEnd(struct Collection const * const collection, unsigned index);

static void * createItem(unsigned key_len, void const * key, void * value,
    uint64_t hash, struct HashMapItem * prev, struct HashMapItem * next);
static void deleteItem(struct HashMapItem * item, CDeleter deleter);

float hash_map_growth_factor = 1.75;


/**
 * Initializes the map
 *
 * @return      the newly created map.
 */
struct HashMap * newHashMap(unsigned initial_capacity) {
    const char * message = "Initial hash map capacity cannot be zero.";
    if (initial_capacity == 0)
        goto exit;

    struct HashMap * map = malloc(sizeof *map);
    if (map == NULL)
       return NULL;

    map -> items = calloc(initial_capacity, sizeof *map -> items);
    if (map -> items == NULL) {
        free(map);
        return NULL;
    }

    struct Collection collection = {
        .get = _hashMapCollectionGet,
        .set = NULL,
        .atEnd = _hashMapCollectionAtEnd,
    };

    map -> collection = collection;
    // At the moment we fix the hash key but in the future we will randomize it
    char hash_key[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
    memcpy(map -> hash_key, hash_key, sizeof(map -> hash_key));
    map -> capacity = initial_capacity;
    map -> size = 0;
    map -> buckets_count = 0;

    return map;

exit:
    fprintf(stderr, "File: %s.\nOperation: newHashMap.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Frees the memory occupied by the map.
 *
 * @param       map pointer to memory occupied by the map.
 */
void deleteHashMap(struct HashMap ** const map, CDeleter deleter) {
    if (map == NULL)
        return;

    if (* map == NULL)
        return;
    
    for (unsigned i = 0; i < (* map) -> capacity; i++) {
        struct HashMapItem * item = (* map) -> items[i];
        if (item == NULL)
            continue;

        struct HashMapItem * next = NULL;
        do {
            next = item -> next;
            deleteItem(item, deleter);
            item = next;
        } while (next != NULL);
    }

    free((* map) -> items);
    free(* map);
    * map = NULL;
}


/**
 * Resizes the given map to higher capacity.
 *
 * @param       map pointer to map to resize.
 * @param       new_capacity the new capacity of the map.
 *
 * @return      the newly resized map.
 */
struct HashMap * resizeHashMap(struct HashMap * const map, unsigned new_capacity) {
    const char * message = NULL;

    if (map == NULL) {
        message = "The parameter <map> cannot be NULL.";
        goto exit;
    }

    if (new_capacity <= map -> capacity) {
        message = "The new capacity cannot less or equal to the existing capacity.";
        goto exit;
    }

    struct HashMapItem ** items = calloc(new_capacity, sizeof *items);
    if (items == NULL)
        return NULL;

    for (unsigned i = 0; i < map -> capacity; i++) {
        struct HashMapItem * item = map -> items[i];
        if (item == NULL)
            continue;
        
        do {
            uint64_t hash = siphash24((char const *) item -> key, item -> key_len, map -> hash_key);
            uint64_t hash_key = hash % new_capacity;
            item -> hash = hash;
            if (items[hash_key] != NULL)
                items[hash_key] -> prev = item;
            item -> next = items[hash_key];
            items[hash_key] = item;
            item = item -> next;
        } while (item != NULL);
    }

    free(map -> items);
    map -> items = items;
    map -> capacity = new_capacity;

    return map;

exit:
    fprintf(stderr, "File: %s.\nOperation: resizeHashMap.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Check if the map is empty.
 *
 * @param       map pointer to the map which content to check.
 *
 * @return      true if the map is empty, false otherwise.
 */
bool isHashMapEmpty(struct HashMap const * const map) {
    char const * message = "The parameter <map> cannot be NULL.";
    if (map == NULL)
        goto exit;

    return map -> size == 0;

exit:
    fprintf(stderr, "File: %s.\nOperation: isHashMapEmpty.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Check if the map is full.
 *
 * @param       map pointer to the map which content to check.
 *
 * @return      true if the map is full, false otherwise.
 */
bool isHashMapFull(struct HashMap const * const map) {
    char const * message = "The parameter <map> cannot be NULL.";
    if (map == NULL)
        goto exit;

    return map -> size == map -> capacity;

exit:
    fprintf(stderr, "File: %s.\nOperation: isHashMapFull.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Inserts a key-value pair into the map.
 *
 * @param       map     pointer to map to append an element to.
 * @param       key_len the length of the key in bytes.
 * @param       key     the key to associate to the value.
 * @param       value   pointer to the value to add to the map.
 */
bool hashMapInsert(struct HashMap * const map, unsigned key_len, void const * key, void * value) {
    char const * message = NULL;

    if (map == NULL) {
        message = "The parameter <map> cannot be NULL.";
        goto exit;
    }

    if (key_len == 0) {
        message = "The key (via key_len) cannot be zero.";
        goto exit;
    }

    // If the load factor exceeds 0.69, we resize the map
    float load_factor = (float)map -> buckets_count / (float)map -> capacity;
    // Maximum load factor pulled from: https://stackoverflow.com/a/31401836
    if (load_factor > 0.693) {
        int new_capacity = map -> capacity * hash_map_growth_factor;
        if (resizeHashMap(map, new_capacity) == NULL)
            return false;
    }

    uint64_t hash = siphash24((char const *) key, key_len, map -> hash_key);
    uint64_t hash_key = hash % map -> capacity;

    struct HashMapItem * existing_item = map -> items[hash_key];
    if (existing_item != NULL && existing_item -> hash == hash) {
        message = "Key already exists in map";
        goto exit;
    }
    // Calculate the load factor
    if (existing_item == NULL)
        map -> buckets_count += 1;

    struct HashMapItem * item = createItem(key_len, key, value, hash, NULL, existing_item);
    if (existing_item != NULL)
        existing_item -> prev = item;
    map -> items[hash_key] = item;
    map -> size++;

    return true;

exit:
    fprintf(stderr, "File: %s.\nOperation: hashMapInsert.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Get the value for the specified key.
 *
 * @param       map pointer to map to use.
 * @param       key the index at which to look.
 *
 * @return      the value associated to the given key.
 */
void * hashMapGet(struct HashMap const * const map, unsigned key_len, void * key) {
    char const * message = NULL;
    
    if (map == NULL) {
        message = "The parameter <map> cannot be NULL.";
        goto exit;
    }

    if (map -> size == 0) {
        message = "The hash map is empty, cannot get elements.";
        goto exit;
    }

    uint64_t hash = siphash24((char const *) key, key_len, map -> hash_key);
    uint64_t hash_key = hash % map -> capacity;
    struct HashMapItem * item = map -> items[hash_key];

    if (item == NULL)
        return NULL;

    bool found = false;
    while (item != NULL) {
        if (item -> hash == hash) {
            found = true;
            break;
        }

        item = item -> next;
    }

    return found ? item -> value : NULL;

exit:
    fprintf(stderr, "File: %s.\nOperation: hashMapGet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/*
 * Changes the value associated to the given key.
 *
 * @param       map     pointer to map to append an element to.
 * @param       key_len the length of the key in bytes.
 * @param       key     the key to associate to the value.
 * @param       value   pointer to the value to add to the map.
 *
 * @return      the value that was replaced if done, otherwise NULL.
 */
void * hashMapSet(struct HashMap * const map, unsigned key_len, void const * key, void * value) {
    char const * message = NULL;

    if (map == NULL) {
        message = "The parameter <map> cannot be NULL.";
        goto exit;
    }

    if (key_len == 0) {
        message = "The key (via key_len) cannot be zero.";
        goto exit;
    }

    uint64_t hash = siphash24((char const *) key, key_len, map -> hash_key);
    uint64_t hash_key = hash % map -> capacity;

    struct HashMapItem * existing_item = map -> items[hash_key];
    if (existing_item == NULL) {
        hashMapInsert(map, key_len, key, value);
        return NULL;
    }
    
    do {
        if (existing_item -> hash == hash) {
            struct HashMapItem * new_item = createItem(
                key_len, key, value, hash,
                existing_item -> prev,  existing_item -> next
            );
            map -> items[hash_key] = new_item;

            void * value = existing_item -> value;
            deleteItem(existing_item, NULL);
            return value;
        }

        existing_item = existing_item -> next;
    } while(existing_item != NULL);

    // If we reach here, then we couldn't find an item with the exact has but the approximate one
    // In this case, the key-value pair needs to be added to the existing items bucket
    hashMapInsert(map, key_len, key, value);
    return NULL;

exit:
    fprintf(stderr, "File: %s.\nOperation: hashMapSet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


/**
 * Removes the key-value pair identified by the given key.
 *
 * @param       map pointer to map to use.
 * @param       key the index at which to look.
 *
 * @return      true if the value was removed, false otherwise (e.g. the key could not be found).
 */
bool hashMapDelete(struct HashMap * const map, unsigned key_len, void * key, CDeleter deleter) {
    char const * message = NULL;

    if (map == NULL) {
        message = "The parameter <map> cannot be NULL.";
        goto exit;
    }

    if (key_len == 0) {
        message = "The key (via key_len) cannot be zero.";
        goto exit;
    }

    uint64_t hash = siphash24((char const *) key, key_len, map -> hash_key);
    uint64_t hash_key = hash % map -> capacity;

    struct HashMapItem * existing_item = map -> items[hash_key];
    if (existing_item == NULL)
        return false;
    
    do {
        if (existing_item -> hash == hash) {
            struct HashMapItem * prev = existing_item -> prev;
            struct HashMapItem * next = existing_item -> next;
            if (prev != NULL) {
                prev -> next = next;
                if (prev -> prev == NULL)
                    map -> items[hash_key] = prev;
            }

            if (next != NULL) {
                next -> prev = prev;
                if (prev == NULL)
                    map -> items[hash_key] = next;
            }

            if (prev == NULL && next == NULL)
                map -> items[hash_key] = NULL;

            deleteItem(existing_item, deleter);

            map -> size--;
            return true;
        }

        existing_item = existing_item -> next;
    } while(existing_item != NULL);

    return false;

exit:
    fprintf(stderr, "File: %s.\nOperation: hashMapDelete.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


static void * _hashMapCollectionGet(struct Collection const * const collection, unsigned index) {
    struct HashMap const * const map = (struct HashMap const * const) collection;
    char const * message = NULL;

    if (map == NULL) {
        message = "The parameter <map> cannot be NULL.";
        goto exit;
    }

    if (map -> size == 0) {
        message = "The hash map is empty, cannot get items.";
        goto exit;
    }

    if (index >= map -> size) {
        message = "The index is out of bounds.";
        goto exit;
    }

    unsigned shadow_index = 0;
    for (int i = 0; i < map -> capacity; i++) {
        struct HashMapItem * item = map -> items[i];
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

exit:
    fprintf(stderr, "File: %s.\nOperation: _hashMapCollectionGet.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


static bool _hashMapCollectionAtEnd(struct Collection const * const collection, unsigned index) {
    struct HashMap const * const map = (struct HashMap const * const) collection;
    const char * message = NULL;

    if (map == NULL) {
        message = "The parameter <map> cannot be NULL.";
        goto exit;
    }

    if (map -> size == 0) {
        message = "The hash  map is empty, cannot check if at end.";
        goto exit;
    }

    return index >= map -> size;

exit:
    fprintf(stderr, "File: %s.\nOperation: _hashMapCollectionAtEnd.\nMessage: %s\n", __FILE__, message);
    exit(74);
}


static void * createItem(unsigned key_len, void const * key, void * value, uint64_t hash, struct HashMapItem * prev, struct HashMapItem * next) {
    struct HashMapItem * item = malloc(sizeof *item);
    if (item == NULL)
        return NULL;
    
    item -> key = key;
    item -> value = value;
    item -> key_len = key_len;
    item -> hash = hash;
    item -> prev = prev;
    item -> next = next;

    return item;
}

static void deleteItem(struct HashMapItem * item, CDeleter deleter) {
    if (deleter != NULL)
        deleter(item -> value);

    free(item);
}
