#include "func.h"

struct FUNC* create_ext_func_one_arg(/*char* name,*/ double (*ext_func)(double))
{
    struct FUNC* func = malloc(sizeof(struct FUNC));
    // func->name = name;
    func->type = FUNC_TYPE_EXTERNAL;

    func->arg_types = malloc(sizeof(enum ARG_TYPE));
    func->arg_types[0] = ARG_TYPE_NUMBER;
    func->arg_types_count = 1;

    func->ext_func_one_arg = ext_func;

    return func;
}

struct FUNC* create_ext_func_two_arg(/*char* name,*/
                                     double (*ext_func)(double, double))
{
    struct FUNC* func = malloc(sizeof(struct FUNC));
    // func->name = name;
    func->type = FUNC_TYPE_EXTERNAL;

    func->arg_types = malloc(2 * sizeof(enum ARG_TYPE));
    func->arg_types[0] = ARG_TYPE_NUMBER;
    func->arg_types[1] = ARG_TYPE_NUMBER;
    func->arg_types_count = 2;

    func->ext_func_two_arg = ext_func;

    return func;
}

enum CALCERR call_func(struct FUNC* func, struct ARG* args, size_t args_count,
                       double* out)
{
    if (args_count != func->arg_types_count) {
        return CALCERR_ARG_COUNT_MISMATCH;
    }

    if (func->type == FUNC_TYPE_EXTERNAL) {
        if (func->arg_types_count == 1) {
            *out = (*func->ext_func_one_arg)(args[0].value);
        } else if (func->arg_types_count == 2) {
            *out = (*func->ext_func_two_arg)(args[0].value, args[1].value);
        } else {
            return CALCERR_ARG_COUNT_MISMATCH;
        }
    }

    return CALCERR_NONE;
}