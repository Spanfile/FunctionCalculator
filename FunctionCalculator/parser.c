#include "parser.h"

int get_precedence(struct PARSER_CONTAINER*);

enum CALCERR create_parser_container(struct TOKEN** tokens, size_t token_count, int* index,
    struct PARSER_CONTAINER** container_out)
{
    *container_out = malloc(sizeof(struct PARSER_CONTAINER));

    if (*container_out == NULL) {
        return CALCERR_MALLOC_FAILED;
    }

    (*container_out)->tokens = tokens;
    (*container_out)->token_count = token_count;
    (*container_out)->index = index;

    return CALCERR_NONE;
}

enum CALCERR parse(struct PARSER_CONTAINER* container, int precedence,
    struct TREE_ELEMENT** elem_out)
{
    if (*container->index >= container->token_count) {
        return CALCERR_UNEXPECTED_END_OF_INPUT;
    }

    struct TOKEN* token = container->tokens[*container->index];
    *container->index += 1;
    struct TREE_ELEMENT* left = NULL;
    enum CALCERR error = CALCERR_NONE;

    switch (token->type) {
    default:
        error = CALCERR_UNEXPECTED_TOKEN;
        break;

    case TOKEN_NAME:
        error = parse_name(token, container, &left);
        break;

    case TOKEN_NUMBER:
        error = parse_number(token, container, &left);
        break;

    case TOKEN_OPEN_BRACKET:
        error = parse_group(token, container, &left);
        break;
    }

    if (error != CALCERR_NONE) {
        return error;
    }

    // no more tokens? we're done
    if (*container->index >= (int)container->token_count) {
        *elem_out = left;
        return CALCERR_NONE;
    }

    while (precedence < get_precedence(container)) {
        token = container->tokens[*container->index];

        switch (token->type) {
        default:
            *elem_out = left;
            return CALCERR_NONE;

        case TOKEN_ADDITION:
        case TOKEN_NEGATION:
        case TOKEN_MULTIPLICATION:
        case TOKEN_DIVISION:
        case TOKEN_POWER:
        case TOKEN_REMAINDER:
            *container->index += 1;

            if ((error = parse_arithmetic(token, left, container, &left)) != CALCERR_NONE) {
                return error;
            }

            break;

        case TOKEN_OPEN_BRACKET:
            *container->index += 1;

            if ((error = parse_function(token, left, container, &left)) != CALCERR_NONE) {
                return error;
            }

            break;
        }
    }

    *elem_out = left;
    return CALCERR_NONE;
}

int get_precedence(struct PARSER_CONTAINER* container)
{
    if (*container->index >= (int)container->token_count) {
        return 0;
    }

    switch (container->tokens[*container->index]->type) {
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