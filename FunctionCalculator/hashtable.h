#pragma once

#include <string.h>
#include <stdlib.h>

struct HASHTABLE_ENTRY {
    char* key;
    void* value_ptr;
    struct HASHTABLE_ENTRY* next;
};

struct HASHTABLE {
    size_t size;
    struct HASHTABLE_ENTRY** buckets;
};

struct HASHTABLE* ht_create(size_t size);
unsigned ht_hash(struct HASHTABLE*, char*);
struct HASHTABLE_ENTRY* ht_newentry(char*, void*);
int ht_set(struct HASHTABLE*, char*, void*);
int ht_get(struct HASHTABLE*, char*, void**);