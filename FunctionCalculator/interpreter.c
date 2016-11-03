#include "interpreter.h"

struct HASHTABLE* names_ht = NULL;
struct HASHTABLE* functions_ht = NULL;

enum CALCERR init_interpreter(void)
{
    names_ht = ht_create(512);
    functions_ht = ht_create(512);

    if (names_ht == NULL) {
        return CALCERR_INTR_INIT_FAILED;
    } else {
        // these math constants may not always be defined
        if (!ht_set(names_ht, "pi", double_to_heap(M_PI))) {
            return CALCERR_INTR_VALUE_SET_FAILED;
        }

        if (!ht_set(names_ht, "e", double_to_heap(M_E))) {
            return CALCERR_INTR_VALUE_SET_FAILED;
        }
    }

    if (functions_ht == NULL) {
        return CALCERR_INTR_INIT_FAILED;
    } else {
        
    }

    return CALCERR_NONE;
}

enum CALCERR free_interpreter(void)
{
    ht_free(names_ht);
    return CALCERR_NONE;
}

enum CALCERR evaluate_element(struct TREE_ELEMENT* element,
                              struct HASHTABLE* extra_names)
{
    enum CALCERR error = CALCERR_NONE;
    switch (element->type) {
    default:
        return CALCERR_INTR_ELEMENT_NOT_IMPLEMENTED;

    case TYPE_NUMBER:
        break; // number elements already have their number value set

    case TYPE_ARITHMETIC:
        if ((error = evaluate_element(element->child1, extra_names)) !=
            CALCERR_NONE) {
            return error;
        }

        if ((error = evaluate_element(element->child2, extra_names)) !=
            CALCERR_NONE) {
            return error;
        }

        double* val = malloc(sizeof(double));
        switch (element->arithmetic_type) {
        case ARITH_ADDITION:
            *val =
                *element->child1->number_value + *element->child2->number_value;
            break;

        case ARITH_NEGATION:
            *val =
                *element->child1->number_value - *element->child2->number_value;
            break;

        case ARITH_MULTIPLICATION:
            *val =
                *element->child1->number_value * *element->child2->number_value;
            break;

        case ARITH_DIVISION:
            if (*element->child2->number_value == 0) {
                return CALCERR_DIVIDE_BY_ZERO;
            }

            *val =
                *element->child1->number_value / *element->child2->number_value;
            break;

        case ARITH_POWER:
            *val = pow(*element->child1->number_value,
                       *element->child2->number_value);
            break;

        case ARITH_REMAINDER:
            *val = remainder(*element->child1->number_value,
                             *element->child2->number_value);
            break;
        }

        element->number_value = val;

        break;

    case TYPE_NAME:
        if (!ht_get(names_ht, element->name_value,
                    (void**)&element->number_value)) {
            if (extra_names != NULL) {
                if (ht_get(extra_names, element->name_value,
                           (void**)&element->number_value)) {
                    break;
                }
            }
            return CALCERR_NAME_NOT_FOUND;
        }

        break;
    }

    return CALCERR_NONE;
}
