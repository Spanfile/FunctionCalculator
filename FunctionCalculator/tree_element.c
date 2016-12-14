#include "tree_element.h"

struct TREE_ELEMENT* create_element(enum TREE_ELEMENT_TYPE type)
{
    struct TREE_ELEMENT* elem = malloc(sizeof(struct TREE_ELEMENT));

    if (elem == NULL) {
        return NULL;
    }

    elem->elem_type = type;

    elem->child1 = NULL;
    elem->child2 = NULL;
    elem->name_value = NULL;
    elem->name_value_len = 0;
    elem->arithmetic_type = ARITH_ADDITION;
    elem->number_value = malloc(sizeof(double));

    if (elem->number_value == NULL) {
        return NULL;
    }

    elem->args = NULL;
    elem->args_len = 0;
    elem->free_number_value = 1;

    // printf("allocate %lu\n", sizeof(*elem->number_value));

    return elem;
}

struct TREE_ELEMENT* create_name_element(char* value, size_t value_len)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_NAME);

    if (elem == NULL) {
        return NULL;
    }

    elem->name_value_len = value_len;
    elem->name_value =
        malloc((value_len + 1) * sizeof(char));

    if (elem->name_value == NULL) {
        return NULL;
    }

    strncpy_s(elem->name_value, (value_len + 1) * sizeof(char), value, value_len * sizeof(char));

    return elem;
}

struct TREE_ELEMENT* create_number_element(double* value)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_NUMBER);

    if (elem == NULL) {
        return NULL;
    }

    free(elem->number_value);
    elem->number_value = value;
    return elem;
}

struct TREE_ELEMENT* create_negation_element(void)
{
    struct TREE_ELEMENT* elem = create_negation_element();

    if (elem == NULL) {
        return NULL;
    }

    return elem;
}

struct TREE_ELEMENT* create_arithmetic_element(enum ARITHMETIC_TYPE type)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_ARITHMETIC);

    if (elem == NULL) {
        return NULL;
    }

    elem->arithmetic_type = type;
    return elem;
}

struct TREE_ELEMENT* create_function_element(char* value, size_t value_len)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_FUNCTION);

    if (elem == NULL) {
        return NULL;
    }

    elem->name_value_len = value_len;
    elem->name_value = malloc((value_len + 1) * sizeof(char));

    if (elem->name_value == NULL) {
        return NULL;
    }

    strncpy_s(elem->name_value, (value_len + 1) * sizeof(char), value, value_len * sizeof(char));

    return elem;
}

struct TREE_ELEMENT* create_assignment_element(char* value, size_t value_len,
                                               enum ASSIGNMENT_TYPE assign_type)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_ASSIGNMENT);

    if (elem == NULL) {
        return NULL;
    }

    elem->assign_type = assign_type;

    elem->name_value_len = value_len;
    elem->name_value = malloc((value_len + 1) * sizeof(char));

    if (elem->name_value == NULL) {
        return NULL;
    }

    strncpy_s(elem->name_value, (value_len + 1) * sizeof(char), value, value_len * sizeof(char));

    return elem;
}

/* returns 1 when successful, 0 if a malloc failed */
int copy_elem(struct TREE_ELEMENT* dst, struct TREE_ELEMENT* src)
{
    /* assume dst has been malloc'd */
    memcpy(dst, src, sizeof(struct TREE_ELEMENT));

    if (src->number_value != NULL) {
        dst->number_value = malloc(sizeof(double));

        if (dst->number_value == NULL) {
            return 0;
        }

        memcpy(dst->number_value, src->number_value, sizeof(double));
    }

    if (src->name_value != NULL) {
        dst->name_value = malloc((src->name_value_len + 1) * sizeof(char));

        if (dst->name_value == NULL) {
            return 0;
        }

        strncpy_s(dst->name_value, (src->name_value_len + 1) * sizeof(char), src->name_value, src->name_value_len * sizeof(char));
    }

    if (dst->args != NULL) {
        dst->args = malloc(dst->args_len * sizeof(struct TREE_ELEMENT*));

        if (dst->args == NULL) {
            return 0;
        }

        for (size_t i = 0; i < dst->args_len; i++) {
            dst->args[i] = malloc(sizeof(struct TREE_ELEMENT));

            if (dst->args[i] == NULL) {
                return 0;
            }

            copy_elem(dst->args[i], src->args[i]);
        }
    }

    if (dst->child1 != NULL) {
        dst->child1 = malloc(sizeof(struct TREE_ELEMENT));

        if (dst->child1 == NULL) {
            return 0;
        }

        if (copy_elem(dst->child1, src->child1) == 0) {
            return 0;
        }
    }

    if (dst->child2 != NULL) {
        dst->child2 = malloc(sizeof(struct TREE_ELEMENT));

        if (dst->child2 == NULL) {
            return 0;
        }

        if (copy_elem(dst->child2, src->child2) == 0) {
            return 0;
        }
    }

    return 1;
}

void free_elem(struct TREE_ELEMENT* elem)
{
    if (elem == NULL) {
        return;
    }

    if (elem->child1 != NULL) {
        free_elem(elem->child1);
    }

    if (elem->child2 != NULL) {
        free_elem(elem->child2);
    }

    if (elem->name_value != NULL) {
        free(elem->name_value);
    }

    if (elem->number_value != NULL && elem->free_number_value) {
        // printf("free %lu\n", sizeof(*elem->number_value));
        free(elem->number_value);
    }

    if (elem->args != NULL) {
        for (size_t i = 0; i < elem->args_len; i++) {
            free_elem(elem->args[i]);
        }

        free(elem->args);
    }

    free(elem);
}
