#include "parser.h"

// element creation
TREE_ELEMENT* create_element(TREE_ELEMENT_TYPE);
TREE_ELEMENT* create_number_element(double);
TREE_ELEMENT* create_arithmetic_element(ARITHMETIC_TYPE);
TREE_ELEMENT* create_name_element(char*, size_t);

// "parselets"
TREE_ELEMENT* parse_name(TOKEN*);

TREE_ELEMENT* parse(TOKEN** tokens, size_t tokens_len, int* index)
{
    TOKEN* token = tokens[*index];

    switch (token->type)
    {
    case TOKEN_NAME:
        return parse_name(token);
    }
}

TREE_ELEMENT* create_element(TREE_ELEMENT_TYPE type)
{
    TREE_ELEMENT* elem = (TREE_ELEMENT*)malloc(sizeof(TREE_ELEMENT));
    elem->type = type;
    return elem;
}

TREE_ELEMENT* create_name_element(char* value, size_t value_len)
{
    TREE_ELEMENT* elem = create_element(TYPE_NAME);
    elem->name_value = value;
    elem->name_value_len = value_len;
    return elem;
}

TREE_ELEMENT* create_number_element(double value)
{
    TREE_ELEMENT* elem = create_element(TYPE_NUMBER);
    elem->number_value = value;
    return elem;
}

TREE_ELEMENT* create_arithmetic_element(ARITHMETIC_TYPE type)
{
    TREE_ELEMENT* elem = create_element(TYPE_ARITHMETIC);
    elem->arithmetic_type = type;
    return elem;
}

TREE_ELEMENT* parse_name(TOKEN* token)
{
    TREE_ELEMENT* elem = create_name_element(token->value, token->value_length);
    return elem;
}
