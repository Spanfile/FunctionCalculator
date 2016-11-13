#include "interpreter.h"

#define FOREACH_EXT_FUNC_ONE_ARG(FUNC)                                         \
    FUNC(sin)                                                                  \
    FUNC(cos)                                                                  \
    FUNC(tan)                                                                  \
    FUNC(asin)                                                                 \
    FUNC(acos)                                                                 \
    FUNC(atan)                                                                 \
    FUNC(sinh)                                                                 \
    FUNC(cosh)                                                                 \
    FUNC(tanh)                                                                 \
    FUNC(log)                                                                  \
    FUNC(log10)                                                                \
    FUNC(sqrt)                                                                 \
    FUNC(floor)                                                                \
    FUNC(ceil)

#ifndef CREATE_FUNC
#define CREATE_FUNC_ONE_ARG(FUNC)                                              \
    if (!ht_set(functions_ht, #FUNC, create_ext_func_one_arg(FUNC))) {         \
        return CALCERR_INTR_VALUE_SET_FAILED;                                  \
    }

#endif

void free_void_func(void* func_ptr)
{
    free_func((struct FUNC*)func_ptr);
}

void free_args(struct ARG** args, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        free(args[i]);
    }

    free(args);
}

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
        FOREACH_EXT_FUNC_ONE_ARG(CREATE_FUNC_ONE_ARG);

        if (!ht_set(functions_ht, "atan2", create_ext_func_two_arg(atan2))) {
            return CALCERR_INTR_VALUE_SET_FAILED;
        }
    }

    return CALCERR_NONE;
}

enum CALCERR free_interpreter(void)
{
    ht_free(names_ht, NULL);
    ht_free(functions_ht, *free_void_func);
    return CALCERR_NONE;
}

enum CALCERR evaluate_element(struct TREE_ELEMENT* element,
                              struct HASHTABLE* extra_names)
{
    enum CALCERR error = CALCERR_NONE;
    struct FUNC* func = NULL;

    switch (element->elem_type) {
    default:
        return CALCERR_INTR_ELEMENT_NOT_IMPLEMENTED;

    case ELEM_NUMBER:
        break; // number elements already have their number value set

    case ELEM_NEGATION:
        if ((error = evaluate_element(element->child1, extra_names)) !=
            CALCERR_NONE) {
            return error;
        }

        *element->number_value = -*element->child1->number_value;
        break;

    case ELEM_ARITHMETIC:
        if ((error = evaluate_element(element->child1, extra_names)) !=
            CALCERR_NONE) {
            return error;
        }

        if ((error = evaluate_element(element->child2, extra_names)) !=
            CALCERR_NONE) {
            return error;
        }

        switch (element->arithmetic_type) {
        case ARITH_ADDITION:
            *element->number_value =
                *element->child1->number_value + *element->child2->number_value;
            break;

        case ARITH_NEGATION:
            *element->number_value =
                *element->child1->number_value - *element->child2->number_value;
            break;

        case ARITH_MULTIPLICATION:
            *element->number_value =
                *element->child1->number_value * *element->child2->number_value;
            break;

        case ARITH_DIVISION:
            if (*element->child2->number_value == 0) {
                return CALCERR_DIVIDE_BY_ZERO;
            }

            *element->number_value =
                *element->child1->number_value / *element->child2->number_value;
            break;

        case ARITH_POWER:
            *element->number_value = pow(*element->child1->number_value,
                                         *element->child2->number_value);
            break;

        case ARITH_REMAINDER:
            *element->number_value = remainder(*element->child1->number_value,
                                               *element->child2->number_value);
            break;
        }

        break;

    case ELEM_NAME:
        free(element->number_value);
        element->free_number_value = 0;
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

    case ELEM_FUNCTION:
        if (!ht_get(functions_ht, element->name_value, (void**)&func)) {
            return CALCERR_NAME_NOT_FOUND;
        }

        for (size_t i = 0; i < element->args_len; i++) {
            if ((error = evaluate_element(element->args[i], extra_names)) !=
                CALCERR_NONE) {
                return error;
            }
        }

        struct ARG** args = NULL;
        if ((error = create_args_from_tree(*element->args, element->args_len,
                                           &args)) != CALCERR_NONE) {
            free_args(args, element->args_len);
            return error;
        }

        if ((error = call_func(func, args, element->args_len,
                               element->number_value)) != CALCERR_NONE) {
            free_args(args, element->args_len);
            return error;
        }

        free_args(args, element->args_len);
        break;
    }

    return CALCERR_NONE;
}
