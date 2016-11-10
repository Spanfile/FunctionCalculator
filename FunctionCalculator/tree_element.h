#pragma once

#include <stdlib.h>
#include <math.h>

#include "ARITHMETIC_TYPE.h"

enum TREE_ELEMENT_TYPE {
    ELEM_TYPE_NUMBER,
    ELEM_TYPE_NEGATION,
    ELEM_TYPE_ARITHMETIC,
    ELEM_TYPE_NAME,
    ELEM_TYPE_FUNCTION
};

struct TREE_ELEMENT {
    enum TREE_ELEMENT_TYPE elem_type;

    // for TYPE_ARITHMETIC
    enum ARITHMETIC_TYPE arithmetic_type;

    // for TYPE_NUMBER
    double* number_value;

    // for TYPE_NAME and TYPE_FUNCTION
    size_t name_value_len;
    char* name_value;

    // for TYPE_FUNCTION
    struct TREE_ELEMENT** args;
    size_t args_len;

    struct TREE_ELEMENT* child1;
    struct TREE_ELEMENT* child2;
};

struct TREE_ELEMENT* create_element(enum TREE_ELEMENT_TYPE);
struct TREE_ELEMENT* create_number_element(double*);
struct TREE_ELEMENT* create_negation_element();
struct TREE_ELEMENT* create_arithmetic_element(enum ARITHMETIC_TYPE);
struct TREE_ELEMENT* create_name_element(char*, size_t);
struct TREE_ELEMENT* create_function_element(char*, size_t,
                                             struct TREE_ELEMENT**, size_t);

void free_elem(struct TREE_ELEMENT*);
