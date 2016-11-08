#pragma once

#include <stdlib.h>

#include "CALCERR.h"

enum ARG_TYPE {
    ARG_TYPE_NUMBER, ARG_TYPE_NAME
};

struct ARG {
    enum ARG_TYPE type;
    double value;
};

int argcmp(const enum ARG_TYPE*, const size_t, const enum ARG_TYPE*, const size_t);
enum CALCERR create_args(const enum ARG_TYPE*, double*, const size_t, struct ARG***);