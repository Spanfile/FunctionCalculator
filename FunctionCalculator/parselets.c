#include "parselets.h"
#include <stdio.h>

enum CALCERR parse_list(struct PARSER_CONTAINER*, struct TREE_ELEMENT***,
                        size_t*);

enum CALCERR parse_name(struct TOKEN* token, struct PARSER_CONTAINER* container,
                        struct TREE_ELEMENT** elem_out)
{
    *elem_out = create_name_element(token->value, token->value_length);
    return CALCERR_NONE;
}

enum CALCERR parse_number(struct TOKEN* token,
                          struct PARSER_CONTAINER* container,
                          struct TREE_ELEMENT** elem_out)
{
    char* tmp;
    *elem_out =
        create_number_element(double_to_heap(strtod(token->value, &tmp)));
    return CALCERR_NONE;
}

enum CALCERR parse_group(struct TOKEN* token,
                         struct PARSER_CONTAINER* container,
                         struct TREE_ELEMENT** inner)
{
    enum CALCERR error = CALCERR_NONE;

    if ((error = parse(container, PRECEDENCE_DEFAULT, inner)) != CALCERR_NONE) {
        return error;
    }

    if (*container->index >= container->token_count) {
        return CALCERR_UNEXPECTED_END_OF_INPUT;
    }

    if (container->tokens[*container->index]->token_type !=
        TOKEN_CLOSE_BRACKET) {
        return CALCERR_EXPECTED_CLOSING_BRACKET;
    }

    *container->index += 1;

    return CALCERR_NONE;
}

enum CALCERR parse_negation(struct TOKEN* token,
                            struct PARSER_CONTAINER* container,
                            struct TREE_ELEMENT** elem_out)
{
    *elem_out = create_negation_element();

    return parse(container, PRECEDENCE_SUM, &((*elem_out)->child1));
}

enum CALCERR parse_arithmetic(struct TOKEN* token, struct TREE_ELEMENT* left,
                              struct PARSER_CONTAINER* container,
                              struct TREE_ELEMENT** elem_out)
{
    enum ARITHMETIC_TYPE type;
    int precedence = 0;

    switch (token->token_type) {
    default:
        return CALCERR_UNEXPECTED_TOKEN;

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

    *elem_out = create_arithmetic_element(type);
    (*elem_out)->child1 = left;

    return parse(container, precedence, &((*elem_out)->child2));
}

enum CALCERR parse_function(struct TOKEN* token, struct TREE_ELEMENT* left,
                            struct PARSER_CONTAINER* container,
                            struct TREE_ELEMENT** elem_out)
{
    if (left->elem_type != ELEM_NAME) {
        return CALCERR_INVALID_ELEMENT;
    }

    *elem_out = create_function_element(left->name_value, left->name_value_len);
    free_elem(left);

    return parse_list(container, &((*elem_out)->args),
                      &((*elem_out)->args_len));
}

enum CALCERR parse_assignment(struct TOKEN* token, struct TREE_ELEMENT* left,
                              struct PARSER_CONTAINER* container,
                              struct TREE_ELEMENT** elem_out)
{
    enum ASSIGNMENT_TYPE assign_type = ASSIGN_NAME;

    switch (left->elem_type) {
    default:
        return CALCERR_INVALID_ELEMENT;

    case ELEM_FUNCTION:
        assign_type = ASSIGN_FUNCTION;
        break;
    }

    *elem_out = create_assignment_element(left->name_value,
                                          left->name_value_len, assign_type);

    if (assign_type == ASSIGN_FUNCTION) {
        (*elem_out)->args_len = left->args_len;
        (*elem_out)->args =
            malloc(left->args_len * sizeof(struct TREE_ELEMENT*));

        for (size_t i = 0; i < left->args_len; i++) {
            (*elem_out)->args[i] = malloc(sizeof(struct TREE_ELEMENT));
            copy_elem((*elem_out)->args[i], left->args[i]);
        }
    }

    free_elem(left);

    return parse(container, PRECEDENCE_DEFAULT, &((*elem_out)->child1));
}

enum CALCERR parse_list(struct PARSER_CONTAINER* container,
                        struct TREE_ELEMENT*** elems, size_t* elem_count)
{
    if (*container->index >= container->token_count) {
        return CALCERR_UNEXPECTED_END_OF_INPUT;
    }

    struct TOKEN* token = NULL;
    enum CALCERR error = CALCERR_NONE;

    size_t elem_array_size = 4;
    *elems = malloc(elem_array_size * sizeof(struct TREE_ELEMENT*));
    *elem_count = 0;

    while (1) {
        if (*container->index >= container->token_count) {
            return CALCERR_UNEXPECTED_END_OF_INPUT;
        }

        token = container->tokens[*container->index];

        if (token->token_type == TOKEN_CLOSE_BRACKET) {
            break;
        }

        if (token->token_type == TOKEN_COMMA) {
            *container->index += 1;
            continue;
        }

        if ((error = parse(container, 0, &(*elems)[*elem_count])) !=
            CALCERR_NONE) {
            for (size_t i = 0; i < elem_array_size; i++) {
                free(*elems[i]);
            }

            free(*elems);
            return error;
        }

        *elem_count += 1;
        if (*elem_count >= elem_array_size) {
            *elems = realloc(*elems, (elem_array_size += 4) *
                                         sizeof(struct TREE_ELEMENT*));
        }
    }

    *container->index += 1;

    return CALCERR_NONE;
}
