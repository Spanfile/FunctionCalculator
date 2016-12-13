#include "parser.h"

int get_precedence(struct PARSER_CONTAINER*);

enum CALCERR create_parser_container(struct TOKEN** tokens, size_t token_count,
                                     int* index,
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
    enum CALCERR error = CALCERR_NONE;

    switch (token->token_type) {
    default:
        error = CALCERR_UNEXPECTED_TOKEN;
        break;

    case TOKEN_NAME:
        error = parse_name(token, container, elem_out);
        break;

    case TOKEN_NUMBER:
        error = parse_number(token, container, elem_out);
        break;

    case TOKEN_NEGATION:
        error = parse_negation(token, container, elem_out);
        break;

    case TOKEN_OPEN_BRACKET:
        error = parse_group(token, container, elem_out);
        break;
    }

    if (error != CALCERR_NONE) {
        return error;
    }

    /* no more tokens? we're done */
    if (*container->index >= (int)container->token_count) {
        return CALCERR_NONE;
    }

    /* precedence is a funny thing. keep parsing tokens as long as the current
       token has a smaller precedence than the one after it */
    while (precedence < get_precedence(container)) {
        token = container->tokens[*container->index];

        switch (token->token_type) {
        default:
            return CALCERR_NONE;

        case TOKEN_ADDITION:
        case TOKEN_NEGATION:
        case TOKEN_MULTIPLICATION:
        case TOKEN_DIVISION:
        case TOKEN_POWER:
        case TOKEN_REMAINDER:
            *container->index += 1;
            error = parse_arithmetic(token, *elem_out, container, elem_out);
            break;

        case TOKEN_OPEN_BRACKET:
            *container->index += 1;
            error = parse_function(token, *elem_out, container, elem_out);
            break;

        case TOKEN_EQUAL:
            *container->index += 1;
            error = parse_assignment(token, *elem_out, container, elem_out);
            break;
        }
    }

    return error;
}

int get_precedence(struct PARSER_CONTAINER* container)
{
    if (*container->index >= (int)container->token_count) {
        return 0;
    }

    /* high precedence = high "importance" */
    switch (container->tokens[*container->index]->token_type) {
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
    case TOKEN_COMMA:
        return PRECEDENCE_GROUP;

    case TOKEN_EQUAL:
        return PRECEDENCE_ASSIGNMENT;
    }
}
