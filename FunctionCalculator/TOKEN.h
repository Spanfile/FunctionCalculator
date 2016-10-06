#pragma once

#include "TOKENTYPE.h"

struct TOKEN {
    TOKENTYPE type;
    size_t value_length;
    char* value;
};
