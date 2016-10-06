#include "TREE_ELEMENT.h"

double get_tree_element_value(TREE_ELEMENT &element)
{
    // calculate a value for the element if it's an arithmetic element
    if (element.type == ARITHMETIC)
    {
        switch (element.arithmetic_type)
        {
        case ADDITION:
            element.value =
                get_tree_element_value(*element.child1) +
                get_tree_element_value(*element.child2);
            break;

        case NEGATION:
            element.value =
                get_tree_element_value(*element.child1) -
                get_tree_element_value(*element.child2);
            break;

        case MULTIPLICATION:
            element.value =
                get_tree_element_value(*element.child1) *
                get_tree_element_value(*element.child2);
            break;

        case DIVISION:
            element.value =
                get_tree_element_value(*element.child1) /
                get_tree_element_value(*element.child2);
            break;

        case POWER:
            element.value = pow(
                get_tree_element_value(*element.child1),
                get_tree_element_value(*element.child2));
            break;

        case REMAINDER:
            element.value = remainder(
                get_tree_element_value(*element.child1),
                get_tree_element_value(*element.child2));
            break;
        }
    }

    return element.value;
}
