#pragma once

#include <stdlib.h>
#include <math.h>

#include "ARITHMETIC_TYPE.h"

#define FOREACH_ELEM_TYPE(ELEM_TYPE)                                           \
    ELEM_TYPE(ELEM_NUMBER)                                                     \
    ELEM_TYPE(ELEM_NEGATION)                                                   \
    ELEM_TYPE(ELEM_ARITHMETIC)                                                 \
    ELEM_TYPE(ELEM_NAME)                                                       \
    ELEM_TYPE(ELEM_FUNCTION)

#ifndef GENERATE_ENUM
#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STR) #STR,
#endif

enum TREE_ELEMENT_TYPE { FOREACH_ELEM_TYPE(GENERATE_ENUM) };

#pragma GCC diagnostic ignored "-Wunused-variable"
static const char* TREE_ELEMENT_TYPE_STRING[] = {
    FOREACH_ELEM_TYPE(GENERATE_STRING)};

struct TREE_ELEMENT {
    enum TREE_ELEMENT_TYPE elem_type;

    // for TYPE_ARITHMETIC
    enum ARITHMETIC_TYPE arithmetic_type;

    // for TYPE_NUMBER
    int free_number_value = 1;
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
