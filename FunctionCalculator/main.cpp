#include "main.h"

int main(void)
{
    //size_t is just typedef'd to an unsigned integer
    size_t read_buffer_size = 32;
    size_t read_len = 0;
    char* read_buffer = (char*)malloc(read_buffer_size);
    char input;

    // read characters into a dynamically expanding buffer
    printf("> ");
    do
    {
        input = getchar();

        // if the buffer is full, resize it by adding 16
        if (read_len == read_buffer_size - 1)
            read_buffer = (char*)realloc(read_buffer, read_buffer_size += 16);

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
