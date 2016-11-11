#include "main.h"

void print_elem(struct TREE_ELEMENT*, int);
void free_tokens(struct TOKEN**, size_t);
void print_error(enum CALCERR, int);

int main(void)
{
    size_t read_buffer_size = 16;
    size_t read_len = 0;
    char* read_buffer = NULL;
    char input;
    int running = 1;
    enum CALCERR error = CALCERR_NONE;

    struct TOKEN** tokens = NULL;
    size_t token_count = 0;
    int index = 0;
    struct PARSER_CONTAINER* container;
    struct TREE_ELEMENT* root_elem;

    if ((error = init_interpreter()) != CALCERR_NONE) {
        printf("error initialising interpreter: %s", CALCERR_STRING[error]);
        return 1; // this is a critical error
    }

    while (running) {
        error = CALCERR_NONE;

        read_buffer_size = 16;
        read_len = 0;
        read_buffer = malloc(read_buffer_size); // TODO: memory check?
        token_count = 0;
        index = 0;

        // read characters into a dynamically expanding buffer
        printf("> ");
        do {
            input = getchar();

            // resize the buffer if it's full
            if (read_len + 1 > read_buffer_size)
                read_buffer = realloc(read_buffer, read_buffer_size += 8);

            read_buffer[read_len++] = input;

        } while (input != '\n');

        // replace the newline at the end with a null terminating byte
        read_buffer[--read_len] = '\0';

        // parse commands
        if (read_buffer[0] == ':' && read_len > 1) {
            enum COMMAND_TYPE cmd = read_buffer[1];
            switch (cmd) {
            default:
                printf("Invalid command: %c\nTry :h for help.\n", cmd);
                continue;

            case COMMAND_QUIT:
                running = 0;
                continue;

            case COMMAND_HELP:
                printf("Commands:\n");
                printf(":h - displays this help\n");
                printf(":q - quits the application\n");
                continue;
            }
        }

        // or parse math
        if ((error = tokenise(read_buffer, read_len, &token_count, &tokens)) !=
            CALCERR_NONE) {
            printf("tokenising error: %s\n", CALCERR_STRING[error]);
        } else if ((error = create_parser_container(tokens, token_count, &index,
                                                    &container)) !=
                   CALCERR_NONE) {
            printf("error: %s\n", CALCERR_STRING[error]);
        } else if ((error = parse(container, 0, &root_elem)) != CALCERR_NONE) {
            printf("parsing error @ i%i: %s\n", index - 1,
                   CALCERR_STRING[error]);
        } else if ((error = evaluate_element(root_elem, NULL)) !=
                   CALCERR_NONE) {
            printf("evaluation error: %s\n", CALCERR_STRING[error]);
        } else {
            printf("%f\n", *root_elem->number_value);
        }

        if (root_elem != NULL) {
            print_elem(root_elem, 0);
            free_elem(root_elem);
            root_elem = NULL;
        }

        free(read_buffer);
        read_buffer = NULL;

        if (tokens != NULL) {
            free_tokens(tokens, token_count);
            tokens = NULL;
        }

        if (container != NULL) {
            free(container);
            container = NULL;
        }
    }

    if ((error = free_interpreter()) != CALCERR_NONE) {
        printf("error freeing interpreter: %s", CALCERR_STRING[error]);
        return 1;
    }

    return 0;
}

void print_elem(struct TREE_ELEMENT* elem, int indent)
{
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }

    printf("%s: ", TREE_ELEMENT_TYPE_STRING[elem->elem_type]);

    switch (elem->elem_type) {
    default:
        printf("unknown");
        break;

    case ELEM_NUMBER:
        printf("%f\n", *elem->number_value);
        break;

    case ELEM_NEGATION:
        printf("\n");
        if (elem->child1 != NULL) {
            print_elem(elem->child1, indent + 1);
        }
        break;

    case ELEM_NAME:
        printf("%s\n", elem->name_value);
        break;

    case ELEM_ARITHMETIC:
        printf("%s\n", ARITHMETIC_TYPE_STRING[elem->arithmetic_type]);

        if (elem->child1 != NULL) {
            print_elem(elem->child1, indent + 1);
        } else {
            printf("missing child1\n");
        }

        if (elem->child2 != NULL) {
            print_elem(elem->child2, indent + 1);
        } else {
            printf("missing child2\n");
        }

        break;

    case ELEM_FUNCTION:
        printf("%s\n", elem->name_value);

        for (size_t i = 0; i < elem->args_len; i++) {
            print_elem(elem->args[i], indent + 1);
        }

        break;
    }
}

void free_tokens(struct TOKEN** tokens, size_t token_count)
{
    for (size_t i = 0; i < token_count; i++) {
        free(tokens[i]);
    }

    free(tokens);
}
