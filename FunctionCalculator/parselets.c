#include "parselets.h"

enum CALCERR parse_name(struct TOKEN* token,
    struct PARSER_CONTAINER* container, struct TREE_ELEMENT** elem_out)
{
    *elem_out = create_name_element(token->value, token->value_length);
    return CALCERR_NONE;
}

enum CALCERR parse_number(struct TOKEN* token,
    struct PARSER_CONTAINER* container, struct TREE_ELEMENT** elem_out)
{
    char* tmp;
    *elem_out = create_number_element(strtod(token->value, &tmp));
    return CALCERR_NONE;
}

enum CALCERR parse_arithmetic(struct TOKEN* token, struct TREE_ELEMENT* left,
    struct PARSER_CONTAINER* container, struct TREE_ELEMENT** elem_out)
{
    enum ARITHMETIC_TYPE type;
    int precedence = 0;
    enum CALCERR error = CALCERR_NONE;

    switch (token->type) {
    default:
        error = CALCERR_UNEXPECTED_TOKEN;
        break;

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

    if (error != CALCERR_NONE) {
        return error;
    }

    *elem_out = create_arithmetic_element(type);
    struct TREE_ELEMENT* right = NULL;
    error = parse(container, precedence, &right);

    if (error != CALCERR_NONE) {
        return error;
    }

    (*elem_out)->child1 = left;
    (*elem_out)->child2 = right;

    return CALCERR_NONE;
}

enum CALCERR parse_group(struct TOKEN* token,
    struct PARSER_CONTAINER* container, struct TREE_ELEMENT** inner)
{
    enum CALCERR error = parse(container, 0, inner);

    if (error != CALCERR_NONE) {
        return error;
    }

    if (*container->index >= container->token_count) {
        return CALCERR_UNEXPECTED_END_OF_INPUT;
    }

    if (container->tokens[*container->index]->type != TOKEN_CLOSE_BRACKET) {
        return CALCERR_EXPECTED_CLOSING_BRACKET;
    }

    *container->index += 1;

    return CALCERR_NONE;
}