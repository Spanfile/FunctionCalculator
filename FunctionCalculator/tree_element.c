#include "tree_element.h"

struct TREE_ELEMENT* create_element(enum TREE_ELEMENT_TYPE type)
{
    struct TREE_ELEMENT* elem = malloc(sizeof(struct TREE_ELEMENT));
    elem->elem_type = type;

    // initialise all fields to default values
    elem->child1 = NULL;
    elem->child2 = NULL;
    elem->name_value = NULL;
    elem->name_value_len = 0;
    elem->arithmetic_type = ARITH_ADDITION;
    elem->number_value = malloc(sizeof(double));
    elem->args = NULL;
    elem->args_len = 0;
    elem->free_number_value = 1;

    // printf("allocate %lu\n", sizeof(*elem->number_value));

    return elem;
}

struct TREE_ELEMENT* create_name_element(char* value, size_t value_len)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_NAME);

    elem->name_value_len = value_len;
    elem->name_value =
        malloc((value_len + 1) * sizeof(char)); // null terminator not included in value
    strncpy_s(elem->name_value, (value_len + 1) * sizeof(char), value, value_len * sizeof(char));

    return elem;
}

struct TREE_ELEMENT* create_number_element(double* value)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_NUMBER);
    free(elem->number_value);
    elem->number_value = value;
    return elem;
}

struct TREE_ELEMENT* create_negation_element(void)
{
    return create_element(ELEM_NEGATION);
}

struct TREE_ELEMENT* create_arithmetic_element(enum ARITHMETIC_TYPE type)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_ARITHMETIC);
    elem->arithmetic_type = type;
    return elem;
}

struct TREE_ELEMENT* create_function_element(char* value, size_t value_len)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_FUNCTION);

    elem->name_value_len = value_len;
    elem->name_value = malloc((value_len + 1) * sizeof(char));
    strncpy_s(elem->name_value, (value_len + 1) * sizeof(char), value, value_len * sizeof(char));

    return elem;
}

struct TREE_ELEMENT* create_assignment_element(char* value, size_t value_len,
                                               enum ASSIGNMENT_TYPE assign_type)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_ASSIGNMENT);
    elem->assign_type = assign_type;

    elem->name_value_len = value_len;
    elem->name_value = malloc((value_len + 1) * sizeof(char));
    strncpy_s(elem->name_value, (value_len + 1) * sizeof(char), value, value_len * sizeof(char));

    return elem;
}

void copy_elem(struct TREE_ELEMENT* dst, struct TREE_ELEMENT* src)
{
    /* we assume dst has been malloc'd */
    memcpy(dst, src, sizeof(struct TREE_ELEMENT));

    if (src->number_value != NULL) {
        dst->number_value = malloc(sizeof(double));
        memcpy(dst->number_value, src->number_value, sizeof(double));
    }

    if (src->name_value != NULL) {
        dst->name_value = malloc((src->name_value_len + 1) * sizeof(char));
        strncpy_s(dst->name_value, (src->name_value_len + 1) * sizeof(char), src->name_value, src->name_value_len * sizeof(char));
    }

    if (dst->args != NULL) {
        dst->args = malloc(dst->args_len * sizeof(struct TREE_ELEMENT*));
        for (size_t i = 0; i < dst->args_len; i++) {
            dst->args[i] = malloc(sizeof(struct TREE_ELEMENT));
            copy_elem(dst->args[i], src->args[i]);
        }
    }

    if (dst->child1 != NULL) {
        dst->child1 = malloc(sizeof(struct TREE_ELEMENT));
        copy_elem(dst->child1, src->child1);
    }

    if (dst->child2 != NULL) {
        dst->child2 = malloc(sizeof(struct TREE_ELEMENT));
        copy_elem(dst->child2, src->child2);
    }
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
