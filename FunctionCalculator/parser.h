#pragma once

#include <stdlib.h>

#include "TOKEN.h"
#include "TREE_ELEMENT.h"

struct PARSER_CONTAINER {
    TOKEN** tokens;
    size_t token_count;
    int* index;
};

PARSER_CONTAINER* create_parser_container(TOKEN**, size_t, int*);
TREE_ELEMENT* parse(PARSER_CONTAINER*);