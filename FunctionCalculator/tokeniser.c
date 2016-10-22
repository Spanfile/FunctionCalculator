#include "tokeniser.h"

int tokenise_name(char*, int*, size_t);
int tokenise_number(char*, int*, size_t);

enum CALCERR tokenise(char* input, size_t input_len,
    size_t* token_count, struct TOKEN*** tokens)
{
    size_t tokens_size = 8;
    *tokens = malloc(tokens_size * sizeof(struct TOKEN*));
    (*token_count) = 0;

    int i = 0;
    do {
        struct TOKEN* token_ptr = malloc(sizeof(struct TOKEN));
        char c = input[i];

        size_t sub_len = 0;
        int read_start = 0;

        if (isalpha(c)) { // names
            int start = i;
            int end = tokenise_name(input, &i, input_len);

            sub_len = end - start;
            read_start = start;

            token_ptr->type = TOKEN_NAME;
        } else if (isdigit(c)) { // numbers
            int start = i;
            int end = tokenise_number(input, &i, input_len);

            sub_len = end - start;
            read_start = start;

            token_ptr->type = TOKEN_NUMBER;
        } else { // everything else
            // note: ugly goto-blocker
            int skip_end = 0;

            switch (c) {
            default:
                return CALCERR_INVALID_CHARACTER;
                break;

            case ' ':
            case '\0':
                continue;

            case '+':
                token_ptr->type = TOKEN_ADDITION;
                break;

            case '-':
                /* special case: if there's a number after the -,
                parse it as a number.
                except that this doesn't work always. you know when that happens */

                if (i < (int)input_len - 1) {
                    char next = input[i + 1];
                    if (isdigit(next)) {
                        i += 1;
                        // start parsing from the number, but include the -
                        int start = i - 1;
                        int end = tokenise_number(input, &i, input_len);

                        sub_len = end - start;
                        read_start = start;
                        
                        token_ptr->type = TOKEN_NUMBER;

                        skip_end = 1;
                        break;
                    }
                }

                token_ptr->type = TOKEN_NEGATION;

                break;

            case '*':
                token_ptr->type = TOKEN_MULTIPLICATION;
                break;

            case '/':
                token_ptr->type = TOKEN_DIVISION;
                break;

            case '^':
                token_ptr->type = TOKEN_POWER;
                break;

            case '%':
                token_ptr->type = TOKEN_REMAINDER;
                break;

            case '(':
                token_ptr->type = TOKEN_OPEN_BRACKET;
                break;

            case ')':
                token_ptr->type = TOKEN_CLOSE_BRACKET;
                break;

            case ',':
                token_ptr->type = TOKEN_COMMA;
                break;
            }

            if (skip_end == 0) {
                /* we techically don't need the value of the token with these
                arithmetic tokens, but ech */
                sub_len = 1;
                read_start = i;
            }
        }

        token_ptr->value_length = sub_len;
        token_ptr->value = malloc(sub_len + 1);
        strncpy(token_ptr->value, &input[read_start], sub_len);
        token_ptr->value[sub_len] = '\0';

        // resize the token array if it's full
        if ((*token_count) + 1 > tokens_size) {
            *tokens = realloc(*tokens, (tokens_size += 8) * sizeof(**tokens));
        } // there may be something fishy going on here

        (*tokens)[*token_count] = token_ptr;
        *token_count += 1;
    } while (i++ < (int)input_len);

    return CALCERR_NONE;
}

int tokenise_name(char* input, int* index, size_t input_len)
{
    char c;

    int end = *index;
    while (1)
    {
        if (*index >= (int)input_len)
            break;

        c = input[*index];
        end += 1;

        c = input[*index + 1];
        if (!isalpha(c) || c == ' ')
            break;

        *index += 1;
    }

    return end;
}

int tokenise_number(char* input, int* index, size_t input_len)
{
    char c;

    int end = *index;
    while (1) {
        if (*index >= (int)input_len) {
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
