#include "pointer_help.h"

double* double_to_heap(double val)
{
    double* ptr = malloc(sizeof(double));
    *ptr = val;
    return ptr;
}