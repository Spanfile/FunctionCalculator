#pragma once

#include <stdlib.h>

#include "tree_element.h"
#include "CALCERR.h"
#include "hashtable.h"
#include "interpreter.h"

enum FUNC_TYPE { FUNC_TYPE_EXTERNAL, FUNC_TYPE_INTERNAL };

struct FUNC {
    char** arg_names;
    size_t arg_count;

    // char* name;

    enum FUNC_TYPE func_type;
    struct TREE_ELEMENT* elem;

    // for external methods (i.e. from math.h)
    double (*ext_func_one_arg)(double);
    double (*ext_func_two_arg)(double, double);
};

struct FUNC* create_ext_func_one_arg(/*char*,*/ double (*ext_func)(double));
struct FUNC* create_ext_func_two_arg(/*char*,*/ double (*ext_func)(double, double));

struct FUNC* create_intr_func(struct TREE_ELEMENT* elem);

enum CALCERR call_func(struct FUNC* func, double** args, size_t args_count,
                       double* out);

void free_func(struct FUNC* func);
