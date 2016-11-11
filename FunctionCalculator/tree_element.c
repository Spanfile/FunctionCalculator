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

    return elem;
}

struct TREE_ELEMENT* create_name_element(char* value, size_t value_len)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_NAME);
    elem->name_value = value;
    elem->name_value_len = value_len;
    return elem;
}

struct TREE_ELEMENT* create_number_element(double* value)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_NUMBER);
    free(elem->number_value);
    elem->number_value = value;
    return elem;
}

struct TREE_ELEMENT* create_negation_element()
{
    struct TREE_ELEMENT* elem = create_element(ELEM_NEGATION);
    return elem;
}

struct TREE_ELEMENT* create_arithmetic_element(enum ARITHMETIC_TYPE type)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_ARITHMETIC);
    elem->arithmetic_type = type;
    return elem;
}

struct TREE_ELEMENT* create_function_element(char* value, size_t value_len,
                                             struct TREE_ELEMENT** args,
                                             size_t args_len)
{
    struct TREE_ELEMENT* elem = create_element(ELEM_FUNCTION);
    elem->name_value = value;
    elem->name_value_len = value_len;
    elem->args = args;
    elem->args_len = args_len;
    return elem;
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

    free(elem);
}
