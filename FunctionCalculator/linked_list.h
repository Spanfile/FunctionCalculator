#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct LINKED_LIST_NODE {
    char* key;
    size_t key_len;
    void* value_ptr;
    size_t value_ptr_len;
    struct LINKED_LIST_NODE* next;
};

struct LINKED_LIST_NODE* ll_newnode(char* key, size_t key_len, void* value, size_t value_len);
int ll_insert(struct LINKED_LIST_NODE* node, char* key, size_t key_len,
              void* value, size_t value_len, int index);
void ll_free(struct LINKED_LIST_NODE* node);
int ll_depth(struct LINKED_LIST_NODE* node);
int ll_tofile(struct LINKED_LIST_NODE* node, char* file);
struct LINKED_LIST_NODE* ll_fromfile(char* file);
