#include "parser.h"

// element creation
TREE_ELEMENT* create_element(TREE_ELEMENT_TYPE);
TREE_ELEMENT* create_number_element(double);
TREE_ELEMENT* create_arithmetic_element(ARITHMETIC_TYPE);
TREE_ELEMENT* create_name_element(char*, size_t);

// "parselets"
TREE_ELEMENT* parse_name(TOKEN*, TOKEN**, size_t, int*);
TREE_ELEMENT* parse_number(TOKEN*, TOKEN**, size_t, int*);
TREE_ELEMENT* parse_arithmetic(TOKEN*, TREE_ELEMENT*, TOKEN**, size_t, int*);

TREE_ELEMENT* parse(TOKEN** tokens, size_t token_count, int* index)
{
    TOKEN* token = tokens[*index];
    (*index) += 1;
    TREE_ELEMENT* left = nullptr;

    switch (token->type)
    {
    case TOKEN_NAME:
        left = parse_name(token, tokens, token_count, index);
        break;

    case TOKEN_NUMBER:
        left = parse_number(token, tokens, token_count, index);
        break;
    }

    if (*index >= (int)token_count)
        return left;

    token = tokens[*index];

    switch (token->type)
    {
    default:
        return left;

    case TOKEN_ADDITION:
    case TOKEN_NEGATION:
    case TOKEN_MULTIPLICATION:
    case TOKEN_DIVISION:
    case TOKEN_POWER:
    case TOKEN_REMAINDER:
        (*index)++;
        return parse_arithmetic(token, left, tokens, token_count, index);
    }
}

TREE_ELEMENT* create_element(TREE_ELEMENT_TYPE type)
{
    TREE_ELEMENT* elem = (TREE_ELEMENT*)malloc(sizeof(TREE_ELEMENT));
    elem->type = type;

    elem->child1 = nullptr;
    elem->child2 = nullptr;

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

TREE_ELEMENT* parse_name(TOKEN* token, TOKEN** tokens,
    size_t token_count, int* index)
{
    TREE_ELEMENT* elem = create_name_element(token->value, token->value_length);
    return elem;
}

TREE_ELEMENT* parse_number(TOKEN* token, TOKEN** tokens,
    size_t token_count, int* index)
{
    char* tmp;
    TREE_ELEMENT* elem = create_number_element(strtod(token->value, &tmp));
    return elem;
}

TREE_ELEMENT* parse_arithmetic(TOKEN* token, TREE_ELEMENT* left, TOKEN** tokens,
    size_t token_count, int* index)
{
    TREE_ELEMENT* right = parse(tokens, token_count, index);
    ARITHMETIC_TYPE type;

    switch (token->type)
    {
    default: // TODO: kinda bad having addition as the default
    case TOKEN_ADDITION:
        type = ARITH_ADDITION;
        break;

    case TOKEN_NEGATION:
        type = ARITH_NEGATION;
        break;

    case TOKEN_MULTIPLICATION:
        type = ARITH_MULTIPLICATION;
        break;

    case TOKEN_DIVISION:
        type = ARITH_DIVISION;
        break;

    case TOKEN_POWER:
        type = ARITH_POWER;
        break;

    case TOKEN_REMAINDER:
        type = ARITH_REMAINDER;
        break;
    }

    TREE_ELEMENT* elem = create_arithmetic_element(type);
    elem->child1 = left;
    elem->child2 = right;
    return elem;
}
