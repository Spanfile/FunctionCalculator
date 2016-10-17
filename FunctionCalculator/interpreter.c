#include "interpreter.h"

double get_tree_element_value(struct TREE_ELEMENT*);

struct hashtable* hashtable = NULL;

void init_interpreter(void)
{
    hashtable = ht_create(512);

    if (hashtable == NULL) {
        printf("Failed to create hashtable");
    } else {
        ht_set(hashtable, "pi", 3.1415927538);
    }
}

double interpret(struct TREE_ELEMENT* root)
{
    return get_tree_element_value(root);
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
        ht_get(hashtable, element->name_value, &element->number_value);
    }

    return element->number_value;
}