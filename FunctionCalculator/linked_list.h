#ifndef _H_LINKED_LIST
#define _H_LINKED_LIST

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
int ll_setval(struct LINKED_LIST_NODE* node, char* key, void* new_val, size_t new_val_len);
void ll_free(struct LINKED_LIST_NODE* node);
int ll_depth(struct LINKED_LIST_NODE* node);
int ll_tofile(struct LINKED_LIST_NODE* node, char* file, void (*custom_write)(FILE* fp, void* value_ptr));
struct LINKED_LIST_NODE* ll_fromfile(char* file, void* (*custom_read)(FILE* fp));

#endif
