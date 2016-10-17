#pragma once

#include <string.h>
#include <stdlib.h>

struct hashtable_entry {
    char* key;
    double value;
    struct hashtable_entry* next;
};

struct hashtable {
    size_t size;
    struct hashtable_entry** buckets;
};

struct hashtable* ht_create(size_t size);
unsigned ht_hash(struct hashtable*, char*);
struct hashtable_entry* ht_newentry(char*, double);
void ht_set(struct hashtable*, char*, double);
void ht_get(struct hashtable*, char*, double*);