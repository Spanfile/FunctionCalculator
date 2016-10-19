#pragma once

#include <stdlib.h>
#include <math.h>

#include "ARITHMETIC_TYPE.h"

enum TREE_ELEMENT_TYPE {
    TYPE_NUMBER, TYPE_ARITHMETIC, TYPE_NAME
};

struct TREE_ELEMENT {
    enum TREE_ELEMENT_TYPE type;

    // for TYPE_ARITHMETIC
    enum ARITHMETIC_TYPE arithmetic_type;

    // for TYPE_NUMBER
    double number_value;

    // for TYPE_NAME
    size_t name_value_len;
    char* name_value;

    struct TREE_ELEMENT* child1;
    struct TREE_ELEMENT* child2;
};

struct TREE_ELEMENT* create_element(enum TREE_ELEMENT_TYPE);
struct TREE_ELEMENT* create_number_element(double);
struct TREE_ELEMENT* create_arithmetic_element(enum ARITHMETIC_TYPE);
struct TREE_ELEMENT* create_name_element(char*, size_t);

void free_elem(struct TREE_ELEMENT*);
