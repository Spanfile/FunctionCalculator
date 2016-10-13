#pragma once

#include "ARITHMETIC_TYPE.h"
#include <stdlib.h>
#include <math.h>

enum TREE_ELEMENT_TYPE {
    TYPE_NUMBER, TYPE_ARITHMETIC, TYPE_NAME
};

struct TREE_ELEMENT {
    TREE_ELEMENT_TYPE type;

    // for TYPE_ARITHMETIC
    ARITHMETIC_TYPE arithmetic_type;

    // for TYPE_NUMBER
    double number_value;

    // for TYPE_NAME
    size_t name_value_len;
    char* name_value;

    TREE_ELEMENT* child1;
    TREE_ELEMENT* child2;
};

double get_tree_element_value(TREE_ELEMENT&);
