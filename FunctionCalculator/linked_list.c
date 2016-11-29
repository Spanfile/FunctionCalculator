#include "linked_list.h"

struct LINKED_LIST_NODE* ll_newnode(char* key, size_t key_len, void* value)
{
    struct LINKED_LIST_NODE* node = malloc(sizeof(struct LINKED_LIST_NODE));

    node->key = malloc(key_len + 1);
    strncpy(node->key, key, key_len);
    node->key[key_len] = '\0';

    node->value_ptr = value;

    return node;
}

int ll_insert(struct LINKED_LIST_NODE* node, char* key, size_t key_len, void* value, int index)
{
    struct LINKED_LIST_NODE* next = node;

    for (int i = 0; i < index; i++) {
        if (next->next == NULL) {
            return 0;
        }

        next = next->next;
    }

    struct LINKED_LIST_NODE* new = ll_newnode(key, key_len, value);
    new->next = next->next;
    next->next = new;

    return 1;
}

void ll_free(struct LINKED_LIST_NODE* node)
{
    if (node->next != NULL) {
        ll_free(node->next);
    }

    free(node->key);
    free(node);
}