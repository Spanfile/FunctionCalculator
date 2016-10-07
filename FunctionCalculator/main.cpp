#include "main.h"

int main(void)
{
    //size_t is just typedef'd to an unsigned integer
    size_t read_buffer_size = 16;
    size_t read_len = 0;
    char* read_buffer = (char*)malloc(read_buffer_size);
    char input;

    // read characters into a dynamically expanding buffer
    printf("> ");
    do
    {
        input = getchar();

        // resize the buffer if it's full
        if (read_len + 1 > read_buffer_size)
            read_buffer = (char*)realloc(read_buffer, read_buffer_size += 8);

        read_buffer[read_len++] = input;

    } while (input != '\n');

    // replace the newline at the end with a null terminating byte
    read_buffer[read_len - 1] = '\0';
    read_len--;

    size_t token_count = 0;
    TOKEN** tokens = tokenise(read_buffer, read_len, &token_count);

    for (int i = 0; i < (int)token_count; i++)
    {
        printf("Token #%d: %d, %s\n", i, tokens[i]->type, tokens[i]->value);

        free(tokens[i]);
    }

    free(tokens);

    getchar();

    return 0;
}
