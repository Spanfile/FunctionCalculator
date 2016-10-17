#include "main.h"

void free_elem(TREE_ELEMENT*);
void print_elem(TREE_ELEMENT*, int);

int main(void)
{
    size_t read_buffer_size = 16;
    size_t read_len = 0;
    char* read_buffer = nullptr;
    char input;
    bool running = true;

    TOKEN** tokens = nullptr;
    size_t token_count = 0;
    int index = 0;
    PARSER_CONTAINER* container;
    TREE_ELEMENT* root_elem;

    while (running)
    {
        read_buffer_size = 16;
        read_len = 0;
        read_buffer = (char*)malloc(read_buffer_size);

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

        // parse commands
        if (read_buffer[0] == ':' && read_len > 1)
        {
            COMMAND_TYPE cmd = (COMMAND_TYPE)read_buffer[1];
            switch (cmd)
            {
            default:
                printf("Invalid command: %c\nTry :h for help.", cmd);
                break;

            case COMMAND_QUIT:
                running = false;
                break;

            case COMMAND_HELP:
                printf("Commands:\n");
                printf(":h - displays this help\n");
                printf(":q - quits the application\n");
                break;
            }
        }
        else // or parse math
        {
            token_count = 0;
            tokens = tokenise(read_buffer, read_len, &token_count);

            free(read_buffer);

            index = 0;
            container = create_parser_container(tokens, token_count, &index);
            root_elem = parse(container, 0);

            free(container);

            print_elem(root_elem, 0);
            free_elem(root_elem);

            for (int i = 0; i < (int)token_count; i++)
                free(tokens[i]);

            free(tokens);
        }
    }

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

void print_elem(TREE_ELEMENT* elem, int indent)
{
    for (int i = 0; i < indent; i++)
        printf(" ");

    printf("%d: ", elem->type);

    switch (elem->type)
    {
    case TYPE_NUMBER:
        printf("%f\n", elem->number_value);
        break;

    case TYPE_NAME:
        printf("%s\n", elem->name_value);
        break;

    case TYPE_ARITHMETIC:
        printf("%d\n", elem->arithmetic_type);

        if (elem->child1 != nullptr)
            print_elem(elem->child1, indent + 1);
        else
            printf("missing child1\n");

        if (elem->child2 != nullptr)
            print_elem(elem->child2, indent + 1);
        else
            printf("missing child2\n");
        break;
    }
}
