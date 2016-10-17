#include "parser.h"
#include "PRECEDENCE.h"

// element creation
TREE_ELEMENT* create_element(TREE_ELEMENT_TYPE);
TREE_ELEMENT* create_number_element(double);
TREE_ELEMENT* create_arithmetic_element(ARITHMETIC_TYPE);
TREE_ELEMENT* create_name_element(char*, size_t);

// "parselets"
TREE_ELEMENT* parse_name(TOKEN*, PARSER_CONTAINER*);
TREE_ELEMENT* parse_number(TOKEN*, PARSER_CONTAINER*);
TREE_ELEMENT* parse_arithmetic(TOKEN*, TREE_ELEMENT*, PARSER_CONTAINER*);
int get_precedence(PARSER_CONTAINER*);

PARSER_CONTAINER* create_parser_container(TOKEN** tokens, size_t token_count, int* index)
{
    PARSER_CONTAINER* container = (PARSER_CONTAINER*)malloc(sizeof(PARSER_CONTAINER));

    container->tokens = tokens;
    container->token_count = token_count;
    container->index = index;

    return container;
}

TREE_ELEMENT* parse(PARSER_CONTAINER* container, int precedence)
{
    TOKEN* token = container->tokens[*container->index];
    *container->index += 1;
    TREE_ELEMENT* left = nullptr;

    switch (token->type)
    {
    default:
        break;

    case TOKEN_NAME:
        left = parse_name(token, container);
        break;

    case TOKEN_NUMBER:
        left = parse_number(token, container);
        break;
    }

    if (*container->index >= (int)container->token_count)
        return left;

    while (precedence < get_precedence(container))
    {
        token = container->tokens[*container->index];

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
            *container->index += 1;
            left = parse_arithmetic(token, left, container);
            break;
        }
    }

    return left;
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

TREE_ELEMENT* parse_name(TOKEN* token, PARSER_CONTAINER* container)
{
    TREE_ELEMENT* elem = create_name_element(token->value, token->value_length);
    return elem;
}

TREE_ELEMENT* parse_number(TOKEN* token, PARSER_CONTAINER* container)
{
    char* tmp;
    TREE_ELEMENT* elem = create_number_element(strtod(token->value, &tmp));
    return elem;
}

TREE_ELEMENT* parse_arithmetic(TOKEN* token, TREE_ELEMENT* left,
    PARSER_CONTAINER* container)
{
    ARITHMETIC_TYPE type;
    int precedence = 0;

    switch (token->type)
    {
    default: // TODO: kinda bad having addition as the default
    case TOKEN_ADDITION:
        type = ARITH_ADDITION;
        precedence = PRECEDENCE_SUM;
        break;

    case TOKEN_NEGATION:
        type = ARITH_NEGATION;
        precedence = PRECEDENCE_SUM;
        break;

    case TOKEN_MULTIPLICATION:
        type = ARITH_MULTIPLICATION;
        precedence = PRECEDENCE_MULT;
        break;

    case TOKEN_DIVISION:
        type = ARITH_DIVISION;
        precedence = PRECEDENCE_MULT;
        break;

    case TOKEN_POWER:
        type = ARITH_POWER;
        precedence = PRECEDENCE_POWER;
        break;

    case TOKEN_REMAINDER:
        type = ARITH_REMAINDER;
        precedence = PRECEDENCE_MULT;
        break;
    }

    TREE_ELEMENT* elem = create_arithmetic_element(type);
    TREE_ELEMENT* right = parse(container, precedence);

    elem->child1 = left;
    elem->child2 = right;
    return elem;
}

int get_precedence(PARSER_CONTAINER* container)
{
    if (*container->index >= (int)container->token_count)
        return 0;

    switch (container->tokens[*container->index]->type)
    {
    default:
        return 0;

    case TOKEN_ADDITION:
    case TOKEN_NEGATION:
        return PRECEDENCE_SUM;

    case TOKEN_MULTIPLICATION:
    case TOKEN_DIVISION:
    case TOKEN_REMAINDER:
        return PRECEDENCE_MULT;

    case TOKEN_POWER:
        return PRECEDENCE_POWER;

    case TOKEN_OPEN_BRACKET:
    case TOKEN_CLOSE_BRACKET:
        return PRECEDENCE_GROUP;
    }
}