#include "hashtable.h"

struct hashtable* ht_create(size_t size)
{
    struct hashtable* ht = NULL;

    ht = malloc(sizeof(struct hashtable));
    if (ht == NULL) {
        return NULL;
    }

    ht->buckets = malloc(sizeof(struct hashtable_entry*) * size);
    if (ht->buckets == NULL) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        ht->buckets[i] = NULL;
    }

    ht->size = size;

    return ht;
}

unsigned ht_hash(struct hashtable* ht, char* key)
{
    // FNV hash
    unsigned char* p = key;
    unsigned h = 2166136261;
    int len = strlen(key);

    for (int i = 0; i < len; i++) {
        h = (h * 16777619) ^ p[i];
    }

    return h;
}

struct hashtable_entry* ht_newentry(char* key, double value)
{
    struct hashtable_entry* entry = malloc(sizeof(struct hashtable_entry));

    if (entry == NULL) {
        return NULL;
    }

    entry->key = strdup(key);
    if (entry->key == NULL) {
        return NULL;
    }

    entry->value = value;
    entry->next = NULL;

    return entry;
}

void ht_set(struct hashtable* ht, char* key, double value) {
    int bucket = 0;
    struct hashtable_entry* new = NULL;
    struct hashtable_entry* next = NULL;
    struct hashtable_entry* last = NULL;

    bucket = ht_hash(ht, key);
    next = ht->buckets[bucket];

    while (next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
        last = next;
        next = next->next; // well that sounds stupid
    }

    // there's an entry with the same key, replace it
    if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {
        next->value = value;
    } else {
        new = ht_newentry(key, value);

        // we're at the start of the linked list in this bucket
        if (next == ht->buckets[bucket]) {
            new->next = next;
            ht->buckets[bucket] = new;
        } else if (next == NULL) { // we're at the end
            last->next = new;
        } else { // we're somewhere in the middle
            new->next = next;
            last->next = new;
        }
    }
}

void ht_get(struct hashtable* ht, char* key, double* out) {
    int bucket = 0;
    struct hashtable_entry* entry = NULL;

    bucket = ht_hash(ht, key);
    entry = ht->buckets[bucket];

    while (entry != NULL && entry->key != NULL && strcmp(key, entry->key) > 0) {
        entry = entry->next;
    }

    if (entry == NULL || entry->key == NULL || strcmp(key, entry->key) != 0) {
        out = NULL;
    }

    *out = entry->value;
}