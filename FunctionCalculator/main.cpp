#include "main.h"

void free_elem(TREE_ELEMENT*);

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
    read_buffer[--read_len] = '\0';

    size_t token_count = 0;
    TOKEN** tokens = tokenise(read_buffer, read_len, &token_count);

    int index = 0;
    TREE_ELEMENT* root_elem = parse(tokens, token_count, &index);

    free_elem(root_elem);

    for (int i = 0; i < (int)token_count; i++)
        free(tokens[i]);

    free(tokens);

    getchar();

    return 0;
}

void free_elem(TREE_ELEMENT* elem)
{
    if (elem->child1 != nullptr)
        free_elem(elem->child1);

    if (elem->child2 != nullptr)
        free_elem(elem->child2);

    free(elem);
}
