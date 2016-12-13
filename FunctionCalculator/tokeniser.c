#include "tokeniser.h"

int tokenise_name(char*, size_t*, size_t);
int tokenise_number(char*, size_t*, size_t);

enum CALCERR tokenise(char* input, size_t input_len, size_t* token_count,
                      struct TOKEN*** tokens)
{
    size_t tokens_size = 8;
    *tokens = malloc(tokens_size * sizeof(struct TOKEN*));
    (*token_count) = 0;

    size_t i = 0;
    do {
        char c = input[i];

        /* these characters don't need a token allocated */
        if (c == ' ' || c == '\0') {
            continue;
        }

        size_t sub_len = 0;
        int read_start = 0;

        /* resize the token array if it's full */
        if ((*token_count) + 1 > tokens_size) {
            *tokens =
                realloc(*tokens, (tokens_size += 8) * sizeof(struct TOKEN*));
        }

        (*tokens)[*token_count] = malloc(sizeof(struct TOKEN));
        struct TOKEN* token_ptr = (*tokens)[*token_count];
        *token_count += 1;
        // printf("allocate %lu @ %p\n", sizeof(*token_ptr), token_ptr);

        if (isalpha(c)) { // names
            int start = i;
            int end = tokenise_name(input, &i, input_len);

            sub_len = end - start;
            read_start = start;

            token_ptr->token_type = TOKEN_NAME;
        } else if (isdigit(c)) {
            /* numbers */
            int start = i;
            int end = tokenise_number(input, &i, input_len);

            sub_len = end - start;
            read_start = start;

            token_ptr->token_type = TOKEN_NUMBER;
        } else {
            /* everything else */
            /* note: skip_end is an ugly goto-blocker */
            int skip_end = 0;

            switch (c) {
            default:
                return CALCERR_INVALID_CHARACTER;

            case '+':
                token_ptr->token_type = TOKEN_ADDITION;
                break;

            case '-':
                token_ptr->token_type = TOKEN_NEGATION;
                break;

            case '*':
                token_ptr->token_type = TOKEN_MULTIPLICATION;
                break;

            case '/':
                token_ptr->token_type = TOKEN_DIVISION;
                break;

            case '^':
                token_ptr->token_type = TOKEN_POWER;
                break;

            case '%':
                token_ptr->token_type = TOKEN_REMAINDER;
                break;

            case '(':
                token_ptr->token_type = TOKEN_OPEN_BRACKET;
                break;

            case ')':
                token_ptr->token_type = TOKEN_CLOSE_BRACKET;
                break;

            case ',':
                token_ptr->token_type = TOKEN_COMMA;
                break;

            case '=':
                token_ptr->token_type = TOKEN_EQUAL;
                break;
            }

            if (skip_end == 0) {
                /* we techically don't need the value of the token with
                   these arithmetic tokens, but ech */
                sub_len = 1;
                read_start = i;
            }
        }

        token_ptr->value_length = sub_len;
        token_ptr->value = malloc((sub_len + 1) * sizeof(char));
        strncpy_s(token_ptr->value, (sub_len + 1) * sizeof(char), &input[read_start], sub_len * sizeof(char));

        // printf("%i @ %i,%i: %s\n", token_ptr->type, i, (int)*token_count,
        // token_ptr->value);
    } while (i++ < input_len);

    return CALCERR_NONE;
}

int tokenise_name(char* input, size_t* index, size_t input_len)
{
    char c;

    int end = *index;
    while (1) {
        if (*index >= input_len)
            break;

        c = input[*index];
        end += 1;

        c = input[*index + 1];
        /* names must start with a letter, but they can then contain numbers
           too */
        if ((!isalpha(c) && !isdigit(c)) || c == ' ')
            break;

        *index += 1;
    }

    return end;
}

int tokenise_number(char* input, size_t* index, size_t input_len)
{
    char c;

    int end = *index;
    while (1) {
        if (*index >= input_len) {
            break;
        }

        c = input[*index];
        end += 1;

        c = input[*index + 1];
        if ((!isdigit(c) && c != '.') || c == ' ') {
            break;
        }

        *index += 1;
    }

    return end;
}

void free_tokens(struct TOKEN*** tokens, size_t token_count)
{
    for (size_t i = 0; i < token_count; i++) {
        // printf("free %lu @ %p\n", sizeof(*(*tokens)[i]), (*tokens)[i]);
        free((*tokens)[i]->value);
        free((*tokens)[i]);
    }

    free(*tokens);
}
