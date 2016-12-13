#ifndef _H_HASHTABLE
#define _H_HASHTABLE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum HASHTABLE_FREE_VALUE { FREE_ENTRY_FALSE, FREE_ENTRY_TRUE };

struct HASHTABLE_ENTRY {
    /* the entries are just linked lists but automatically sorted with their key.
       even though there is an existing implementation for linked list, this
       hashtable implements its own internal linked list for the sorting. */
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
unsigned ht_hash(struct HASHTABLE* ht, char* key);
struct HASHTABLE_ENTRY* ht_newentry(char* key, size_t key_len, void* value_ptr,
                                    enum HASHTABLE_FREE_VALUE free_value);
int ht_set(struct HASHTABLE* ht, char* key, size_t key_len, void* value_ptr,
           void (*free_entry)(void*), enum HASHTABLE_FREE_VALUE free_value);
int ht_get(struct HASHTABLE* ht, char* key, void** out);
int ht_remove(struct HASHTABLE* ht, char* key, void (*free_entry)(void*));
void ht_free(struct HASHTABLE* ht, void (*free_entry)(void*));

#endif
