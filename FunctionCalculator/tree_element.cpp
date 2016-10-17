#include "tree_element.h"

TREE_ELEMENT* create_element(TREE_ELEMENT_TYPE type)
{
    TREE_ELEMENT* elem = (TREE_ELEMENT*)malloc(sizeof(TREE_ELEMENT));
    elem->type = type;

    elem->child1 = nullptr;
    elem->child2 = nullptr;

    return elem;
}

TREE_ELEMENT* create_name_element(char* value, size_t value_len)
{
    TREE_ELEMENT* elem = create_element(TYPE_NAME);
    elem->name_value = value;
    elem->name_value_len = value_len;
    return elem;
}

TREE_ELEMENT* create_number_element(double value)
{
    TREE_ELEMENT* elem = create_element(TYPE_NUMBER);
    elem->number_value = value;
    return elem;
}

TREE_ELEMENT* create_arithmetic_element(ARITHMETIC_TYPE type)
{
    TREE_ELEMENT* elem = create_element(TYPE_ARITHMETIC);
    elem->arithmetic_type = type;
    return elem;
}

//double get_tree_element_value(TREE_ELEMENT &element)
//{
//    // calculate a value for the element if it's an arithmetic element
//    if (element.type == TYPE_ARITHMETIC)
//    {
//        switch (element.arithmetic_type)
//        {
//        case ARITH_ADDITION:
//            element.number_value =
//                get_tree_element_value(*element.child1) +
//                get_tree_element_value(*element.child2);
//            break;
//
//        case ARITH_NEGATION:
//            element.number_value =
//                get_tree_element_value(*element.child1) -
//                get_tree_element_value(*element.child2);
//            break;
//
//        case ARITH_MULTIPLICATION:
//            element.number_value =
//                get_tree_element_value(*element.child1) *
//                get_tree_element_value(*element.child2);
//            break;
//
//        case ARITH_DIVISION:
//            element.number_value =
//                get_tree_element_value(*element.child1) /
//                get_tree_element_value(*element.child2);
//            break;
//
//        case ARITH_POWER:
//            element.number_value = pow(
//                get_tree_element_value(*element.child1),
//                get_tree_element_value(*element.child2));
//            break;
//
//        case ARITH_REMAINDER:
//            element.number_value = remainder(
//                get_tree_element_value(*element.child1),
//                get_tree_element_value(*element.child2));
//            break;
//        }
//    }
//
//    return element.number_value;
//}
