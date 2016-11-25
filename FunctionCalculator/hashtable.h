#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct HASHTABLE_ENTRY {
    char* key;
    void* value_ptr;
    int free_value;
    struct HASHTABLE_ENTRY* next;
};

struct HASHTABLE {
    size_t size;
    struct HASHTABLE_ENTRY** buckets;
};

struct HASHTABLE* ht_create(size_t size);
unsigned ht_hash(struct HASHTABLE*, char*);
struct HASHTABLE_ENTRY* ht_newentry(char*, size_t, void*, int);
int ht_set(struct HASHTABLE*, char*, size_t, void*, void (*)(void*), int);
int ht_get(struct HASHTABLE*, char*, void**);
void ht_free(struct HASHTABLE*, void (*)(void*));