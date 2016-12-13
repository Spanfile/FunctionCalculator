#include "pointer_help.h"

double* double_to_heap(double val)
{
    double* ptr = malloc(sizeof(val));

    if (ptr == NULL) {
        return NULL;
    }

    *ptr = val;
    return ptr;
}