#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

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

        if (isalpha(c)) // names
        {
            int start = i;
            int end = start;
            while (true)
            {
                if (i >= (int)len)
                    break;

                c = input[i];

                if (!isalpha(c) || c == ' ')
                    break;

                i++;
                end++;
            }

            size_t sub_len = end - start;

            token_ptr->value_length = sub_len;
            token_ptr->value = (char*)malloc(sub_len + 1);
            strncpy_s(token_ptr->value, sub_len + 1, &input[start], sub_len);

            token_ptr->type = NAME;
        }
        else if (isdigit(c)) // numbers
        {

        }
        else // everything else
        {
            switch (c)
            {
            default:
                // TODO
                break;

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

            size_t size = 1;
            token_ptr->value_length = size;
            token_ptr->value = (char*)malloc(size + 1);
            strncpy_s(token_ptr->value, size + 1, &input[i], size);
        }

        if (*token_count == tokens_size - 1)
            tokens = (TOKEN**)realloc(tokens, (tokens_size += 16) * sizeof(*tokens));

        tokens[(*token_count)++] = token_ptr;
    } while (i++ < (int)len);

    return tokens;
}