#include "hashtable.h"

struct HASHTABLE* ht_create(size_t size)
{
    struct HASHTABLE* ht = NULL;

    ht = malloc(sizeof(struct HASHTABLE));
    if (ht == NULL) {
        return NULL;
    }

    ht->size = size;

    ht->buckets = malloc(sizeof(struct HASHTABLE_ENTRY*) * ht->size);
    if (ht->buckets == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < ht->size; i++) {
        ht->buckets[i] = NULL;
    }

    return ht;
}

unsigned ht_hash(struct HASHTABLE* ht, char* key)
{
    // FNV hash
    unsigned char* u_key = (unsigned char*)key;
    unsigned h = 2166136261;
    int len = strlen(key);

    for (int i = 0; i < len; i++) {
        h = (h * 16777619) ^ u_key[i];
    }

    // printf("hash %s: %lu\n", key, h % ht->size);
    return h % ht->size;
}

struct HASHTABLE_ENTRY* ht_newentry(char* key, size_t key_len, void* value_ptr,
                                    enum HASHTABLE_FREE_VALUE free_value)
{
    struct HASHTABLE_ENTRY* entry = malloc(sizeof(struct HASHTABLE_ENTRY));

    if (entry == NULL) {
        return NULL;
    }

    entry->key = malloc((key_len + 1) * sizeof(char));
    strncpy_s(entry->key, (key_len + 1) * sizeof(char), key, key_len * sizeof(char));

    entry->value_ptr = value_ptr;
    entry->free_value = free_value;
    entry->next = NULL;

    return entry;
}

int ht_set(struct HASHTABLE* ht, char* key, size_t key_len, void* value_ptr,
           void (*free_entry)(void*), enum HASHTABLE_FREE_VALUE free_value)
{
    int bucket = 0;
    struct HASHTABLE_ENTRY* new = NULL;
    struct HASHTABLE_ENTRY* next = NULL;
    struct HASHTABLE_ENTRY* last = NULL;

    bucket = ht_hash(ht, key);
    next = ht->buckets[bucket];

    /* go through all the entries in the linked list, find a position for the
     * new key */
    while (next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
        last = next;
        next = next->next; // well that sounds stupid
    }

    /* found the exact same key; replace it */
    if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {
        if (next->free_value) {
            if (free_entry != NULL) {
                (*free_entry)(next->value_ptr);
            } else {
                free(next->value_ptr);
            }
        }

        next->value_ptr = value_ptr;
    } else {
        /* else create a new entry */
        new = ht_newentry(key, key_len, value_ptr, free_value);

        if (new == NULL) {
            return 0;
        }

        /* it goes in the start */
        if (next == ht->buckets[bucket]) {
            new->next = next;
            ht->buckets[bucket] = new;
        } else if (next == NULL) { /* it goes in the end */
            /* this and the next one are hash collisions */
            printf("hash collision with %s and %s: %u\n", key, last->key,
                   bucket);
            last->next = new;
        } else { /* it goes somewhere in the middle */
            printf("hash collision with %s and %s: %u\n", key, last->key,
                   bucket);
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

                if (entry->free_value) {
                    if (free_entry != NULL) {
                        (*free_entry)(entry->value_ptr);
                    } else {
                        free(entry->value_ptr);
                    }
                }

                free(entry->key);
                free(entry);
                entry = next;
            }
        }
    }

    free(ht->buckets);
    free(ht);
    ht = NULL;
}
