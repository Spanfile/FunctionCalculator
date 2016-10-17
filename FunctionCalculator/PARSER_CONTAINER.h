#pragma once

#include <stdlib.h>

#include "TOKEN.h"

struct PARSER_CONTAINER {
    struct TOKEN** tokens;
    size_t token_count;
    int* index;
};