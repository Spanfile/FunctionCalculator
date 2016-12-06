#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#include "tree_element.h"
#include "hashtable.h"
#include "CALCERR.h"
#include "pointer_help.h"
#include "func.h"
#include "linked_list.h"

#define FOREACH_EXT_FUNC_ONE_ARG(FUNC)                                         \
    FUNC(sin)                                                                  \
    FUNC(cos)                                                                  \
    FUNC(tan)                                                                  \
    FUNC(asin)                                                                 \
    FUNC(acos)                                                                 \
    FUNC(atan)                                                                 \
    FUNC(sinh)                                                                 \
    FUNC(cosh)                                                                 \
    FUNC(tanh)                                                                 \
    FUNC(log)                                                                  \
    FUNC(log10)                                                                \
    FUNC(sqrt)                                                                 \
    FUNC(floor)                                                                \
    FUNC(ceil)

#ifndef CREATE_FUNC
#define CREATE_FUNC_ONE_ARG(FUNC)                                              \
    if (!ht_set(functions_ht, #FUNC, strlen(#FUNC),                            \
                create_ext_func_one_arg(FUNC), NULL, FREE_ENTRY_TRUE)) {       \
        return CALCERR_VALUE_SET_FAILED;                                       \
    }

#endif

#ifndef GENERATE_STRING
#define GENERATE_STRING(STR) #STR,
#endif

int is_name_reserved(char*);

enum CALCERR init_interpreter(void);
enum CALCERR free_interpreter(void);
enum CALCERR load_uservalues(void);
enum CALCERR save_uservalues(void);
// extra_names is used for function calls
// user values are added to the main name hashtable
enum CALCERR evaluate_element(struct TREE_ELEMENT*, struct HASHTABLE*);

enum CALCERR add_ans(double);
enum CALCERR get_ans(int, double*);
void print_ans();
enum CALCERR save_ans();
enum CALCERR load_ans();
