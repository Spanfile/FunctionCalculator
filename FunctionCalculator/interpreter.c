#include "interpreter.h"

double get_tree_element_value(struct TREE_ELEMENT*);

double interpret(struct TREE_ELEMENT* root)
{
    return 0;
}

double get_tree_element_value(struct TREE_ELEMENT* element)
{
    // calculate a value for the element if it's an arithmetic element
    if (element->type == TYPE_ARITHMETIC) {
        switch (element->arithmetic_type) {
        case ARITH_ADDITION:
            element->number_value =
                get_tree_element_value(element->child1) +
                get_tree_element_value(element->child2);
            break;

        case ARITH_NEGATION:
            element->number_value =
                get_tree_element_value(element->child1) -
                get_tree_element_value(element->child2);
            break;

        case ARITH_MULTIPLICATION:
            element->number_value =
                get_tree_element_value(element->child1) *
                get_tree_element_value(element->child2);
            break;

        case ARITH_DIVISION:
            element->number_value =
                get_tree_element_value(element->child1) /
                get_tree_element_value(element->child2);
            break;

        case ARITH_POWER:
            element->number_value = pow(
                get_tree_element_value(element->child1),
                get_tree_element_value(element->child2));
            break;

        case ARITH_REMAINDER:
            element->number_value = remainder(
                get_tree_element_value(element->child1),
                get_tree_element_value(element->child2));
            break;
        }
    } else if (element->type == TYPE_NAME) {
        
    }

    return element->number_value;
}