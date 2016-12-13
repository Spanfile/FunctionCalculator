#include "linked_list.h"

struct LINKED_LIST_NODE* ll_newnode(char* key, size_t key_len, void* value,
                                    size_t value_len)
{
    struct LINKED_LIST_NODE* node = malloc(sizeof(struct LINKED_LIST_NODE));

    node->key_len = key_len;
    node->key = malloc((key_len + 1) * sizeof(char));
    strncpy_s(node->key, (key_len + 1) * sizeof(char), key, key_len * sizeof(char));

    node->value_ptr = value;
    node->value_ptr_len = value_len;
    node->next = NULL;

    return node;
}

int ll_insert(struct LINKED_LIST_NODE* node, char* key, size_t key_len,
              void* value, size_t value_len, int index)
{
    struct LINKED_LIST_NODE* next = node;

    for (int i = 0; i < index; i++) {
        if (next->next == NULL) {
            return 0;
        }

        next = next->next;
    }

    struct LINKED_LIST_NODE* new = ll_newnode(key, key_len, value, value_len);
    new->next = next->next;
    next->next = new;

    return 1;
}

int ll_setval(struct LINKED_LIST_NODE* node, char* key, void* new_val,
              size_t new_val_len)
{
    struct LINKED_LIST_NODE* next = node;

    while (strcmp(key, next->key) != 0) {
        next = next->next;

        if (next == NULL) {
            return 0;
        }
    }

    node->value_ptr = new_val;
    node->value_ptr_len = new_val_len;

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

int ll_depth(struct LINKED_LIST_NODE* node)
{
    int depth = 0;
    struct LINKED_LIST_NODE* next = node;

    do {
        depth += 1;
    } while ((next = next->next) != NULL);

    return depth;
}

int ll_tofile(struct LINKED_LIST_NODE* node, char* file,
              void (*custom_write)(FILE* fp, void* value_ptr))
{
    FILE* fp = NULL;

    fopen_s(&fp, file, "wb");
    if (fp == NULL) {
        return 0;
    }

    /* depth is stored at the beginning of the file, but it's only known after
    writing all the nodes */
    fseek(fp, sizeof(int), SEEK_SET);

    int depth = 0;
    struct LINKED_LIST_NODE* next = node;
    do {
        fwrite(&next->key_len, sizeof(next->key_len), 1, fp);
        fwrite(next->key, next->key_len, 1, fp);

        if (custom_write != NULL) {
            custom_write(fp, next->value_ptr);
        } else {
            fwrite(&next->value_ptr_len, sizeof(next->value_ptr_len), 1, fp);
            fwrite(next->value_ptr, next->value_ptr_len, 1, fp);
        }

        depth += 1;
    } while ((next = next->next) != NULL);

    fseek(fp, 0, SEEK_SET);
    fwrite(&depth, sizeof(int), 1, fp);

    fclose(fp);

    return 1;
}

struct LINKED_LIST_NODE* ll_fromfile(char* file, void* (*custom_read)(FILE* fp))
{
    FILE* fp = NULL;

    fopen_s(&fp, file, "rb");
    if (fp == NULL) {
        return NULL;
    }

    int depth = 0;
    fread(&depth, sizeof(int), 1, fp);

    struct LINKED_LIST_NODE* first = NULL;
    struct LINKED_LIST_NODE* prev = NULL;

    size_t key_len;
    char* key;
    size_t value_ptr_len;
    void* value_ptr;

    for (int i = 0; i < depth; i++) {
        fread(&key_len, sizeof(key_len), 1, fp);
        key = malloc(key_len);
        fread(key, key_len, 1, fp);

        if (custom_read != NULL) {
            value_ptr = custom_read(fp);
        } else {
            fread(&value_ptr_len, sizeof(value_ptr_len), 1, fp);
            value_ptr = malloc(value_ptr_len);
            fread(value_ptr, value_ptr_len, 1, fp);
        }

        if (first == NULL) {
            first = ll_newnode(key, key_len, value_ptr, value_ptr_len);
            prev = first;
        } else {
            prev->next = ll_newnode(key, key_len, value_ptr, value_ptr_len);
            prev = prev->next;
        }

        free(key);
    }

    fclose(fp);

    return first;
}