#include "hashtable.h"

struct HASHTABLE* ht_create(size_t size)
{
    struct HASHTABLE* ht = NULL;

    ht = malloc(sizeof(struct HASHTABLE));
    if (ht == NULL) {
        return NULL;
    }

    ht->buckets = malloc(sizeof(struct HASHTABLE_ENTRY*) * size);
    if (ht->buckets == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        ht->buckets[i] = NULL;
    }

    ht->size = size;

    return ht;
}

unsigned ht_hash(struct HASHTABLE* ht, char* key)
{
    // FNV hash
    unsigned char* u_key = key;
    unsigned h = 2166136261;
    int len = strlen(key);

    for (int i = 0; i < len; i++) {
        h = (h * 16777619) ^ u_key[i];
    }

    return h % ht->size;
}

struct HASHTABLE_ENTRY* ht_newentry(char* key, void* value_ptr)
{
    struct HASHTABLE_ENTRY* entry = malloc(sizeof(struct HASHTABLE_ENTRY));

    if (entry == NULL) {
        return NULL;
    }

    entry->key = key;
    entry->value_ptr = value_ptr;
    entry->next = NULL;

    return entry;
}

int ht_set(struct HASHTABLE* ht, char* key, void* value_ptr)
{
    int bucket = 0;
    struct HASHTABLE_ENTRY* new = NULL;
    struct HASHTABLE_ENTRY* next = NULL;
    struct HASHTABLE_ENTRY* last = NULL;

    bucket = ht_hash(ht, key);
    next = ht->buckets[bucket];

    while (next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
        last = next;
        next = next->next; // well that sounds stupid
    }

    if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {
        next->value_ptr = value_ptr;
    } else {
        new = ht_newentry(key, value_ptr);

        if (new == NULL) {
            return 0;
        }

        if (next == ht->buckets[bucket]) {
            new->next = next;
            ht->buckets[bucket] = new;
        } else if (next == NULL) {
            last->next = new;
        } else {
            new->next = next;
            last->next = new;
        }
    }

    return 1;
}

int ht_get(struct HASHTABLE* ht, char* key, void** out)
{
    int bucket = 0;
    struct HASHTABLE_ENTRY* entry = NULL;

    bucket = ht_hash(ht, key);
    entry = ht->buckets[bucket];

    while (entry != NULL && entry->key != NULL && strcmp(key, entry->key) > 0) {
        entry = entry->next;
    }

    if (entry == NULL || entry->key == NULL || strcmp(key, entry->key) != 0) {
        return 0;
    }

    *out = entry->value_ptr;
    return 1;
}

void ht_free(struct HASHTABLE* ht, void (*free_entry)(void*))
{
    struct HASHTABLE_ENTRY* entry = NULL;
    struct HASHTABLE_ENTRY* next = NULL;

    for (size_t i = 0; i < ht->size; i++) {
        if ((entry = ht->buckets[i]) != NULL) {
            while (entry != NULL) {
                next = entry->next;

                if (free_entry != NULL) {
                    (*free_entry)(entry->value_ptr);
                } else {
                    free(entry->value_ptr);
                }

                free(entry);
                entry = next;
            }
        }
    }

    free(ht->buckets);
    free(ht);
    ht = NULL;
}
