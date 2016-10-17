#include "parselets.h"

struct TREE_ELEMENT* parse_name(struct TOKEN* token, struct PARSER_CONTAINER* container)
{
    struct TREE_ELEMENT* elem = create_name_element(token->value, token->value_length);
    return elem;
}

struct TREE_ELEMENT* parse_number(struct TOKEN* token, struct PARSER_CONTAINER* container)
{
    char* tmp;
    struct TREE_ELEMENT* elem = create_number_element(strtod(token->value, &tmp));
    return elem;
}

struct TREE_ELEMENT* parse_arithmetic(struct TOKEN* token, struct TREE_ELEMENT* left,
    struct PARSER_CONTAINER* container)
{
    enum ARITHMETIC_TYPE type;
    int precedence = 0;

    switch (token->type) {
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

    struct TREE_ELEMENT* elem = create_arithmetic_element(type);
    struct TREE_ELEMENT* right = parse(container, precedence);

    elem->child1 = left;
    elem->child2 = right;
    return elem;
}