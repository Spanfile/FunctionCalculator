#pragma once

#include "ARITHMETIC_TYPE.h"
#include <math.h>

enum TREE_ELEMENT_TYPE {
    NUMBER, ARITHMETIC
};

struct TREE_ELEMENT {
    TREE_ELEMENT_TYPE type;

    // for TREE_ELEMENT_TYPE.ARITHMETIC
    ARITHMETIC_TYPE arithmetic_type;

    double value;
    TREE_ELEMENT* child1;
    TREE_ELEMENT* child2;
};

double get_tree_element_value(struct TREE_ELEMENT&);
