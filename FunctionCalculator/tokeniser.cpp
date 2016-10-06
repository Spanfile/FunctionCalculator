#include "tokeniser.h"

TOKEN** tokenise(char* input, size_t len, size_t* token_count)
{
    size_t tokens_size = 16;
    TOKEN** tokens = (TOKEN**)malloc(tokens_size * sizeof(TOKEN));
    (*token_count) = 0;

    int i = 0;
    do
    {
        TOKEN* token_ptr = new TOKEN();
        char c = input[i];

        size_t sub_len = 0;
        int read_start = 0;

        if (isalpha(c)) // names
        {
            int start = i;
            int end = start;
            while (true)
            {
                if (i >= (int)len)
                    break;

                c = input[i];
                end++;

                char next = input[i + 1];
                if (!isalpha(next) || next == ' ')
                    break;

                i++;
            }

            sub_len = end - start;
            read_start = start;

            token_ptr->type = NAME;
        }
        else if (isdigit(c)) // numbers
        {
            int start = i;
            int end = start;
            while (true)
            {
                if (i >= (int)len)
                    break;

                c = input[i];
                end++;

                char next = input[i + 1];
                if ((!isdigit(next) && next != '.') || next == ' ')
                    break;

                i++;
            }

            sub_len = end - start;
            read_start = start;

            token_ptr->type = NUMBER;
        }
        else // everything else
        {
            switch (c)
            {
            default:
                // TODO
                printf("Unknown character found (i %d): %c", i, c);
                break;

            case ' ':
            case '\0':
                continue;

            case '+':
                token_ptr->type = ADDITION;
                break;

            case '-':
                token_ptr->type = NEGATION;
                break;

            case '*':
                token_ptr->type = MUILTIPLICATION;
                break;

            case '/':
                token_ptr->type = DIVISION;
                break;

            case '^':
                token_ptr->type = POWER;
                break;

            case '%':
                token_ptr->type = REMAINDER;
                break;

            case '(':
                token_ptr->type = OPEN_BRACKET;
                break;

            case ')':
                token_ptr->type = CLOSE_BRACKET;
                break;
            }

            /* we techically don't need the value of the token with these
            arithmetic tokens, but ech */
            sub_len = 1;
            read_start = i;
        }

        token_ptr->value_length = sub_len;
        token_ptr->value = (char*)malloc(sub_len + 1);
        strncpy_s(token_ptr->value, sub_len + 1, &input[read_start], sub_len);

        if (*token_count == tokens_size - 1)
            tokens = (TOKEN**)realloc(tokens, (tokens_size += 16) * sizeof(*tokens));

        tokens[(*token_count)++] = token_ptr;
    } while (i++ < (int)len);

    return tokens;
}
