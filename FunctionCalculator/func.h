#pragma once

#include "arg.h"

enum FUNC_TYPE { FUNC_TYPE_EXTERNAL, FUNC_TYPE_INTERNAL };

struct FUNC {
    enum ARG_TYPE* arg_types;
    size_t arg_types_count;

    //char* name;

    enum FUNC_TYPE func_type;

    // for external methods (i.e. from math.h)
    double (*ext_func_one_arg)(double);
    double (*ext_func_two_arg)(double, double);
};

struct FUNC* create_ext_func_one_arg(/*char*,*/ double (*)(double));
struct FUNC* create_ext_func_two_arg(/*char*,*/ double (*)(double, double));

enum CALCERR call_func(struct FUNC*, struct ARG**, size_t, double*);