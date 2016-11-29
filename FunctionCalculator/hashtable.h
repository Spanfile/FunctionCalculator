#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum HASHTABLE_FREE_VALUE { FREE_ENTRY_FALSE, FREE_ENTRY_TRUE };

/* the entries are just linked lists but automatically sorted with their key.
 * this hashtable implements its own internal linked list for the sorting. */
struct HASHTABLE_ENTRY {
    char* key;
    void* value_ptr;
    enum HASHTABLE_FREE_VALUE free_value;
    struct HASHTABLE_ENTRY* next;
};

struct HASHTABLE {
    size_t size;
    struct HASHTABLE_ENTRY** buckets;
};

struct HASHTABLE* ht_create(size_t size);
unsigned ht_hash(struct HASHTABLE*, char*);
struct HASHTABLE_ENTRY* ht_newentry(char*, size_t, void*,
                                    enum HASHTABLE_FREE_VALUE);
int ht_set(struct HASHTABLE*, char*, size_t, void*, void (*)(void*),
           enum HASHTABLE_FREE_VALUE);
int ht_get(struct HASHTABLE*, char*, void**);
void ht_free(struct HASHTABLE*, void (*)(void*));