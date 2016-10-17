#include "tree_element.h"

struct TREE_ELEMENT* create_element(enum TREE_ELEMENT_TYPE type)
{
    struct TREE_ELEMENT* elem = (struct TREE_ELEMENT*)malloc(sizeof(struct TREE_ELEMENT));
    elem->type = type;

    elem->child1 = NULL;
    elem->child2 = NULL;

    return elem;
}

struct TREE_ELEMENT* create_name_element(char* value, size_t value_len)
{
    struct TREE_ELEMENT* elem = create_element(TYPE_NAME);
    elem->name_value = value;
    elem->name_value_len = value_len;
    return elem;
}

struct TREE_ELEMENT* create_number_element(double value)
{
    struct TREE_ELEMENT* elem = create_element(TYPE_NUMBER);
    elem->number_value = value;
    return elem;
}

struct TREE_ELEMENT* create_arithmetic_element(enum ARITHMETIC_TYPE type)
{
    struct TREE_ELEMENT* elem = create_element(TYPE_ARITHMETIC);
    elem->arithmetic_type = type;
    return elem;
}
