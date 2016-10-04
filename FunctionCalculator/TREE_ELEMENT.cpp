#include "TREE_ELEMENT.h"

double get_value_of_tree_element(TREE_ELEMENT &element)
{
    switch (element.type)
    {
    case NUMBER:
        return element.value;

    case ARITHMETIC:
        switch (element.arithmetic_type)
        {
        case ADDITION:
            element.value =
                get_value_of_tree_element(*element.child1) +
                get_value_of_tree_element(*element.child2);
            break;

        case NEGATION:
            element.value =
                get_value_of_tree_element(*element.child1) -
                get_value_of_tree_element(*element.child2);
            break;

        case MULTIPLICATION:
            element.value =
                get_value_of_tree_element(*element.child1) *
                get_value_of_tree_element(*element.child2);
            break;

        case DIVISION:
            element.value =
                get_value_of_tree_element(*element.child1) /
                get_value_of_tree_element(*element.child2);
            break;

        case POWER:
            element.value = pow(
                get_value_of_tree_element(*element.child1),
                get_value_of_tree_element(*element.child2));
            break;

        case REMAINDER:
            element.value = remainder(
                get_value_of_tree_element(*element.child1),
                get_value_of_tree_element(*element.child2));
            break;
        }

        return element.value;
    }
}