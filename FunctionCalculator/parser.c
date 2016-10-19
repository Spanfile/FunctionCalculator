#include "parser.h"

int get_precedence(struct PARSER_CONTAINER*);

struct PARSER_CONTAINER* create_parser_container(struct TOKEN** tokens, size_t token_count, int* index)
{
    struct PARSER_CONTAINER* container = malloc(sizeof(struct PARSER_CONTAINER));

    container->tokens = tokens;
    container->token_count = token_count;
    container->index = index;

    return container;
}

struct TREE_ELEMENT* parse(struct PARSER_CONTAINER* container, int precedence)
{
    struct TOKEN* token = container->tokens[*container->index];
    *container->index += 1;
    struct TREE_ELEMENT* left = NULL;

    switch (token->type) {
    default:
        break;

    case TOKEN_NAME:
        left = parse_name(token, container);
        break;

    case TOKEN_NUMBER:
        left = parse_number(token, container);
        break;
    }

    if (*container->index >= (int)container->token_count) {
        return left;
    }

    while (precedence < get_precedence(container)) {
        token = container->tokens[*container->index];

        switch (token->type) {
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