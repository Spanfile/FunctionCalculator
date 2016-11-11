#include "arg.h"

int argcmp(const enum ARG_TYPE* args1, const size_t args1_len,
           const enum ARG_TYPE* args2, const size_t args2_len)
{
    size_t len = args1_len;
    if (len > args2_len) {
        len = args2_len;
    }

    for (int i = 0; i < len; i++) {
        if (args1[i] != args2[i]) {
            return 0;
        }
    }

    return 1;
}

enum CALCERR create_args_from_tree(const struct TREE_ELEMENT* elems,
                                   const size_t len, struct ARG*** out)
{
    *out = malloc(len * sizeof(struct ARG));

    for (int i = 0; i < len; i++) {
        (*out)[i] = malloc(sizeof(struct ARG));

        switch (elems[i].elem_type) {
        default:
            return CALCERR_ARG_CONVERSION_FAILED;

        case ELEM_NAME:
            (*out)[i]->arg_type = ARG_TYPE_NAME;
            break;

        case ELEM_ARITHMETIC:
        case ELEM_FUNCTION:
        case ELEM_NUMBER:
            (*out)[i]->arg_type = ARG_TYPE_NUMBER;
            (*out)[i]->value = *elems[i].number_value;
            break;
        }
    }

    return CALCERR_NONE;
}