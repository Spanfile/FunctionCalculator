#include "interpreter.h"

struct HASHTABLE* names_ht = NULL;

enum CALCERR init_interpreter(void)
{
    names_ht = ht_create(512);

    if (names_ht == NULL) {
        return CALCERR_INTR_INIT_FAILED;
    } else {
        // these math constants may not always be defined
        if (!ht_set(names_ht, "pi", &(double){M_PI})) {
            return CALCERR_INTR_VALUE_SET_FAILED;
        }

        if (!ht_set(names_ht, "e", &(double){M_E})) {
            return CALCERR_INTR_VALUE_SET_FAILED;
        }
    }

    return CALCERR_NONE;
}

enum CALCERR evaluate_element(struct TREE_ELEMENT* element, struct HASHTABLE* extra_names)
{
    enum CALCERR error = CALCERR_NONE;
    switch (element->type) {
    default:
        return CALCERR_INTR_ELEMENT_NOT_IMPLEMENTED;
        
    case TYPE_NUMBER:
        break; // number elements already have their number value set

    case TYPE_ARITHMETIC:
        if ((error = evaluate_element(element->child1, extra_names)) != CALCERR_NONE) {
            return error;
        }

        if ((error = evaluate_element(element->child2, extra_names)) != CALCERR_NONE) {
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

        break;

    case TYPE_NAME:
        if (!ht_get(names_ht, element->name_value, &element->number_value)) {
            if (extra_names != NULL) {
                if (ht_get(extra_names, element->name_value, &element->number_value)) {
                    break;
                }
            }
            return CALCERR_NAME_NOT_FOUND;
        }

        break;
    }

    return CALCERR_NONE;
}