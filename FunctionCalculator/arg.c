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

enum CALCERR create_args(const enum ARG_TYPE* types, double* values,
                         const size_t len, struct ARG*** out)
{
    *out = malloc(len * sizeof(struct ARG));

    for (int i = 0; i < len; i++) {
        (*out)[i] = malloc(sizeof(struct ARG));
        (*out)[i]->type = types[i];
        (*out)[i]->value = values[i];
    }

    return CALCERR_NONE;
}