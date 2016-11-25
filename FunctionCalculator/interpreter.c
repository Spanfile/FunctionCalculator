#include "interpreter.h"

static const char* RESERVED_NAMES[] = {
    FOREACH_EXT_FUNC_ONE_ARG(GENERATE_STRING) "atan2", "pi", "e", "ans"};
static const size_t RESERVED_NAMES_COUNT =
    sizeof(RESERVED_NAMES) / sizeof(RESERVED_NAMES[0]);

double get_ans_double(double);
void free_func_void(void*);

struct HASHTABLE* names_ht = NULL;
struct HASHTABLE* functions_ht = NULL;

double* ans_array = NULL;
size_t ans_count;
size_t ans_len;

enum CALCERR func_error = CALCERR_NONE;

double get_ans_double(double index_d)
{
    int index = (int)index_d;
    double out = 0;
    enum CALCERR error = CALCERR_NONE;

    if ((error = get_ans(index, &out)) != CALCERR_NONE) {
        func_error = error;
        return 0;
    }

    return out;
}

int is_name_reserved(char* name)
{
    for (size_t i = 0; i < RESERVED_NAMES_COUNT; i++) {
        if (strcmp(name, RESERVED_NAMES[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

enum CALCERR init_interpreter(void)
{
    // printf("%lu reserved names\n", RESERVED_NAMES_COUNT);

    ans_count = 0;
    ans_len = 4;
    ans_array = malloc(ans_len * sizeof(double));

    names_ht = ht_create(128);
    functions_ht = ht_create(128);

    if (names_ht == NULL) {
        return CALCERR_INTR_INIT_FAILED;
    } else {
        // these math constants may not always be defined
        if (!ht_set(names_ht, "pi", 2, double_to_heap(M_PI), NULL,
                    FREE_ENTRY_TRUE)) {
            return CALCERR_VALUE_SET_FAILED;
        }

        if (!ht_set(names_ht, "e", 1, double_to_heap(M_E), NULL,
                    FREE_ENTRY_TRUE)) {
            return CALCERR_VALUE_SET_FAILED;
        }

        /* when answers are added to the array, they're pushed to the front.
        this way the array functions as a pointer to the most recent answer */
        if (!ht_set(names_ht, "ans", 3, ans_array, NULL, FREE_ENTRY_FALSE)) {
            return CALCERR_VALUE_SET_FAILED;
        }
    }

    if (functions_ht == NULL) {
        return CALCERR_INTR_INIT_FAILED;
    } else {
        FOREACH_EXT_FUNC_ONE_ARG(CREATE_FUNC_ONE_ARG);

        if (!ht_set(functions_ht, "ans", 3,
                    create_ext_func_one_arg(get_ans_double), NULL,
                    FREE_ENTRY_TRUE)) {
            return CALCERR_VALUE_SET_FAILED;
        }

        if (!ht_set(functions_ht, "atan2", 5, create_ext_func_two_arg(atan2),
                    NULL, FREE_ENTRY_TRUE)) {
            return CALCERR_VALUE_SET_FAILED;
        }
    }

    return CALCERR_NONE;
}

enum CALCERR free_interpreter(void)
{
    ht_free(names_ht, NULL);
    ht_free(functions_ht, free_func_void);
    free(ans_array);
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

        if (extra_names != NULL) {
            if (ht_get(extra_names, element->name_value,
                       (void**)&element->number_value)) {
                break;
            }
        }

        if (!ht_get(names_ht, element->name_value,
                    (void**)&element->number_value)) {
            return CALCERR_NAME_NOT_FOUND;
        }

        break;

    case ELEM_FUNCTION:
        if (!ht_get(functions_ht, element->name_value, (void**)&func)) {
            return CALCERR_NAME_NOT_FOUND;
        }

        double** args = malloc(element->args_len * sizeof(double*));
        for (size_t i = 0; i < element->args_len; i++) {
            if ((error = evaluate_element(element->args[i], extra_names)) !=
                CALCERR_NONE) {
                return error;
            }

            args[i] = malloc(sizeof(double));
            *args[i] = *element->args[i]->number_value;
        }

        func_error =
            call_func(func, args, element->args_len, element->number_value);

        for (size_t i = 0; i < element->args_len; i++) {
            free(args[i]);
        }

        free(args);

        if (func_error != CALCERR_NONE) {
            return func_error;
        }

        break;

    case ELEM_ASSIGNMENT:
        if (is_name_reserved(element->name_value)) {
            return CALCERR_NAME_RESERVED;
        }

        switch (element->assign_type) {
        case ASSIGN_NAME:
            if ((error = evaluate_element(element->child1, NULL)) !=
                CALCERR_NONE) {
                return error;
            }

            if (!ht_set(names_ht, element->name_value, element->name_value_len,
                        double_to_heap(*element->child1->number_value), NULL,
                        FREE_ENTRY_TRUE)) {
                return CALCERR_VALUE_SET_FAILED;
            }
            break;

        case ASSIGN_FUNCTION:
            for (size_t i = 0; i < element->args_len; i++) {
                if (element->args[i]->elem_type != ELEM_NAME) {
                    return CALCERR_ARG_TYPE_MISMATCH;
                }
            }

            if (!ht_set(functions_ht, element->name_value,
                        element->name_value_len, create_intr_func(element),
                        NULL, FREE_ENTRY_TRUE)) {
                return CALCERR_VALUE_SET_FAILED;
            }

            *element->child1->number_value = 0;
            break;
        }

        *element->number_value = *element->child1->number_value;
        break;
    }

    return CALCERR_NONE;
}

enum CALCERR add_ans(double ans)
{
    if (ans_count >= ans_len) {
        ans_array = realloc(ans_array, (ans_len += 4) * sizeof(double));
        // reallocating changed the pointer, update it
        ht_set(names_ht, "ans", 3, ans_array, NULL, FREE_ENTRY_FALSE);
    }

    if (ans_count > 0) {
        memmove(ans_array + 1, ans_array, ans_count * sizeof(double));
    }

    ans_count += 1;
    ans_array[0] = ans;

    return CALCERR_NONE;
}

enum CALCERR get_ans(int index, double* out)
{
    if (index > ans_count) {
        return CALCERR_INVALID_ANS_INDEX;
    }

    *out = ans_array[index];
    return CALCERR_NONE;
}

void print_ans()
{
    for (size_t i = 0; i < ans_count; i++) {
        printf("%f", ans_array[i]);

        if (i < ans_count - 1) {
            printf(", ");
        }
    }

    printf("\n");
}

void free_func_void(void* func_void)
{
    free_func((struct FUNC*)func_void);
}
