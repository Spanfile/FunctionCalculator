#pragma once

#include "arg.h"

enum EXTERNAL_FUNC_ARG_COUNT {
    EXTERNAL_FUNC_ARG_COUNT_ONE = 1,
    EXTERNAL_FUNC_ARG_COUNT_TWO
};

struct FUNC {
    enum ARG_TYPE* arg_types;

    char* name;

    // for external methods (i.e. from math.h)
    enum EXTERNAL_FUNC_ARG_COUNT ext_arg_count;
    double (*ext_func_one_arg)(double);
    double (*ext_func_two_arg)(double, double);
};

struct FUNC create_ext_func_one_arg(char*, double (*)(double));
struct FUNC create_ext_func_two_arg(char*, double (*)(double, double));