#pragma once

#include <stdlib.h>

#include "CALCERR.h"
#include "tree_element.h"

enum ARG_TYPE {
    ARG_TYPE_NUMBER, ARG_TYPE_NAME
};

struct ARG {
    enum ARG_TYPE arg_type;
    double value;
};

int argcmp(const enum ARG_TYPE*, const size_t, const enum ARG_TYPE*, const size_t);
enum CALCERR create_args_from_tree(const struct TREE_ELEMENT*, const size_t, struct ARG***);