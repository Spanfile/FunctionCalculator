#include "parselets.h"
#include <stdio.h>

enum CALCERR parse_list(struct PARSER_CONTAINER*, struct TREE_ELEMENT***, size_t*);

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
    *elem_out = create_number_element(double_to_heap(strtod(token->value, &tmp)));
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

    if ((error = parse(container, precedence, &right)) != CALCERR_NONE) {
        return error;
    }

    (*elem_out)->child1 = left;
    (*elem_out)->child2 = right;

    return CALCERR_NONE;
}

enum CALCERR parse_function(struct TOKEN* token, struct TREE_ELEMENT* left,
    struct PARSER_CONTAINER* container, struct TREE_ELEMENT** elem_out)
{
    if (left->type != TYPE_NAME) {
        return CALCERR_INVALID_ELEMENT;
    }

    enum CALCERR error = CALCERR_NONE;
    struct TREE_ELEMENT** args = NULL;
    size_t elem_count = 0;

    if ((error = parse_list(container, &args, &elem_count)) != CALCERR_NONE) {
        return error;
    }

    *elem_out = create_function_element(left->name_value, left->name_value_len, args, elem_count);
    return CALCERR_NONE;
}

enum CALCERR parse_list(struct PARSER_CONTAINER* container,
    struct TREE_ELEMENT*** elems, size_t* elem_count)
{
    struct TOKEN* token = NULL;
    enum CALCERR error = CALCERR_NONE;

    size_t elem_array_size = 4;
    *elems = malloc(elem_array_size * sizeof(struct TREE_ELEMENT*));
    *elem_count = 0;

    while ((token = container->tokens[*container->index])->type != TOKEN_CLOSE_BRACKET) {
        switch (token->type) {
        default:
            return CALCERR_UNEXPECTED_TOKEN;

        case TOKEN_COMMA: // commas act as separators
            *container->index += 1;
            continue;

        case TOKEN_NAME:
            error = parse_name(token, container, &(*elems)[*elem_count]);
            break;

        case TOKEN_NUMBER:
            error = parse_number(token, container, &(*elems)[*elem_count]);
            break;
        }

        if (error != CALCERR_NONE) {
            for (int i = 0; i < elem_array_size; i++) {
                free(*elems[i]);
            }

            free(*elems);
            return error;
        }

        *elem_count += 1;
        if (*elem_count >= elem_array_size) {
            *elems = realloc(*elems, (elem_array_size += 4) * sizeof(struct TREE_ELEMENT*));
        }

        *container->index += 1;
    }

    return CALCERR_NONE;
}
