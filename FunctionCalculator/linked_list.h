#pragma once

#include <stdlib.h>
#include <string.h>

struct LINKED_LIST_NODE {
    char* key;
    void* value_ptr;
    struct LINKED_LIST_NODE* next;
};

struct LINKED_LIST_NODE* ll_newnode(char* key, size_t key_len, void* value);
int ll_insert(struct LINKED_LIST_NODE* node, char* key, size_t key_len,
              void* value, int index);
void ll_free(struct LINKED_LIST_NODE* node);
