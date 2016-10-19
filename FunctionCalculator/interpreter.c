#include "interpreter.h"

struct hashtable* hashtable = NULL;

enum CALCERR init_interpreter(void)
{
    hashtable = ht_create(512);

    if (hashtable == NULL) {
        return CALCERR_INTERPRETER_INIT_FAILED;
    } else {
        // these constants may not always be defined
        if (!ht_set(hashtable, "pi", M_PI)) {
            return CALCERR_INTERPRETER_VALUE_SET_FAILED;
        }

        if (!ht_set(hashtable, "e", M_E)) {
            return CALCERR_INTERPRETER_VALUE_SET_FAILED;
        }
    }

    return CALCERR_NONE;
}

enum CALCERR evaluate_element(struct TREE_ELEMENT* element)
{
    // calculate a value for the element if it's an arithmetic element
    if (element->type == TYPE_ARITHMETIC) {
        enum CALCERR error = evaluate_element(element->child1);

        if (error != CALCERR_NONE) {
            return error;
        }

        error = evaluate_element(element->child2);

        if (error != CALCERR_NONE) {
            return error;
        }

        switch (element->arithmetic_type) {
        case ARITH_ADDITION:
            element->number_value =
                element->child1->number_value +
                element->child2->number_value;
            break;

        case ARITH_NEGATION:
            element->number_value =
                element->child1->number_value -
                element->child2->number_value;
            break;

        case ARITH_MULTIPLICATION:
            element->number_value =
                element->child1->number_value *
                element->child2->number_value;
            break;

        case ARITH_DIVISION:
            if (element->child2->number_value == 0) {
                return CALCERR_DIVIDE_BY_ZERO;
            }

            element->number_value =
                element->child1->number_value /
                element->child2->number_value;
            break;

        case ARITH_POWER:
            element->number_value = pow(
                element->child1->number_value,
                element->child2->number_value);
            break;

        case ARITH_REMAINDER:
            element->number_value = remainder(
                element->child1->number_value,
                element->child2->number_value);
            break;
        }
    } else if (element->type == TYPE_NAME) {
        ht_get(hashtable, element->name_value, &element->number_value);
    }

    return CALCERR_NONE;
}