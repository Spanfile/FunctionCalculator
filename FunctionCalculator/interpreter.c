#include "interpreter.h"

static const char* RESERVED_NAMES[] = {
    FOREACH_EXT_FUNC_ONE_ARG(GENERATE_STRING) "atan2", "pi", "e", "ans"};
static const size_t RESERVED_NAMES_COUNT =
    sizeof(RESERVED_NAMES) / sizeof(RESERVED_NAMES[0]);

double get_ans_double(double);
void free_func_void(void*);

struct HASHTABLE* names_ht = NULL;
struct HASHTABLE* functions_ht = NULL;

struct LINKED_LIST_NODE* uservalues_ll = NULL;

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

    enum CALCERR error = CALCERR_NONE;
    if ((error = load_ans()) != CALCERR_NONE) {
        printf("ans loading failed (%s)\n", CALCERR_STRING[error]);
    }

    names_ht = ht_create(128);
    functions_ht = ht_create(128);

    if (names_ht == NULL) {
        return CALCERR_INTR_INIT_FAILED;
    } else {
        // these math constants may not always be defined
        if (!ht_set(names_ht, "pi", 2, double_to_heap(3.1415928538), NULL,
                    FREE_ENTRY_TRUE)) {
            return CALCERR_VALUE_SET_FAILED;
        }

        if (!ht_set(names_ht, "e", 1, double_to_heap(2.71828), NULL,
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
    /* ans_array is freed with names_ht */

    if (uservalues_ll != NULL) {
        ll_free(uservalues_ll);
    }

    return CALCERR_NONE;
}

enum CALCERR load_uservalues(void)
{
    if ((uservalues_ll = ll_fromfile("uservalues.bin", NULL)) != NULL) {
        struct LINKED_LIST_NODE* node = uservalues_ll;
        do {
            if (!ht_set(names_ht, node->key, node->key_len, node->value_ptr,
                        NULL, FREE_ENTRY_TRUE)) {
                return CALCERR_VALUE_SET_FAILED;
            }
        } while ((node = node->next) != NULL);
    }

    return CALCERR_NONE;
}

enum CALCERR save_uservalues(void)
{
    if (uservalues_ll != NULL) {
        if (!ll_tofile(uservalues_ll, "uservalues.bin", NULL)) {
            return CALCERR_USERVALUE_SAVING_FAILED;
        }
    }

    return CALCERR_NONE;
}

enum CALCERR evaluate_element(struct TREE_ELEMENT* elem,
                              struct HASHTABLE* extra_names)
{
    /* this whole method is basically for settings an element's number_value
     * properly */

    enum CALCERR error = CALCERR_NONE;
    struct FUNC* func = NULL;

    switch (elem->elem_type) {
    default:
        return CALCERR_INTR_ELEMENT_NOT_IMPLEMENTED;

    case ELEM_NUMBER:
        break; // number elements already have their number value set

    case ELEM_NEGATION:
        if ((error = evaluate_element(elem->child1, extra_names)) !=
            CALCERR_NONE) {
            return error;
        }

        *elem->number_value = -*elem->child1->number_value;
        break;

    case ELEM_ARITHMETIC:
        if ((error = evaluate_element(elem->child1, extra_names)) !=
            CALCERR_NONE) {
            return error;
        }

        if ((error = evaluate_element(elem->child2, extra_names)) !=
            CALCERR_NONE) {
            return error;
        }

        switch (elem->arithmetic_type) {
        case ARITH_ADDITION:
            *elem->number_value =
                *elem->child1->number_value + *elem->child2->number_value;
            break;

        case ARITH_NEGATION:
            *elem->number_value =
                *elem->child1->number_value - *elem->child2->number_value;
            break;

        case ARITH_MULTIPLICATION:
            *elem->number_value =
                *elem->child1->number_value * *elem->child2->number_value;
            break;

        case ARITH_DIVISION:
            if (*elem->child2->number_value == 0) {
                return CALCERR_DIVIDE_BY_ZERO;
            }

            *elem->number_value =
                *elem->child1->number_value / *elem->child2->number_value;
            break;

        case ARITH_POWER:
            *elem->number_value = pow(*elem->child1->number_value,
                                         *elem->child2->number_value);
            break;

        case ARITH_REMAINDER:
            *elem->number_value = remainder(*elem->child1->number_value,
                                               *elem->child2->number_value);
            break;
        }

        break;

    case ELEM_NAME:
        free(elem->number_value);
        elem->free_number_value = 0;

        if (extra_names != NULL) {
            if (ht_get(extra_names, elem->name_value,
                       (void**)&elem->number_value)) {
                break;
            }
        }

        if (!ht_get(names_ht, elem->name_value,
                    (void**)&elem->number_value)) {
            return CALCERR_NAME_NOT_FOUND;
        }

        break;

    case ELEM_FUNCTION:
        if (!ht_get(functions_ht, elem->name_value, (void**)&func)) {
            return CALCERR_NAME_NOT_FOUND;
        }

        double** args = malloc(elem->args_len * sizeof(double*));
        for (size_t i = 0; i < elem->args_len; i++) {
            if ((error = evaluate_element(elem->args[i], extra_names)) !=
                CALCERR_NONE) {

                // this method could be generalised
                for (size_t j = 0; j < i; j++) {
                    free(args[j]);
                }

                free(args);

                return error;
            }

            args[i] = malloc(sizeof(double));
            *args[i] = *elem->args[i]->number_value;
        }

        func_error =
            call_func(func, args, elem->args_len, elem->number_value);

        for (size_t i = 0; i < elem->args_len; i++) {
            free(args[i]);
        }

        free(args);

        if (func_error != CALCERR_NONE) {
            return func_error;
        }

        break;

    case ELEM_ASSIGNMENT:
        if (is_name_reserved(elem->name_value)) {
            return CALCERR_NAME_RESERVED;
        }

        switch (elem->assign_type) {
        case ASSIGN_NAME:
            if ((error = evaluate_element(elem->child1, NULL)) !=
                CALCERR_NONE) {
                return error;
            }

            double* val = double_to_heap(*elem->child1->number_value);

            if (!ht_set(names_ht, elem->name_value, elem->name_value_len,
                        val, NULL, FREE_ENTRY_TRUE)) {
                free(val);
                return CALCERR_VALUE_SET_FAILED;
            }

            if (uservalues_ll == NULL) {
                uservalues_ll =
                    ll_newnode(elem->name_value, elem->name_value_len,
                               (void*)val, sizeof(*val));
            } else {
                /* first try setting an existing name */
                if (!ll_setval(uservalues_ll, elem->name_value,
                    (void*)val, sizeof(*val))) {
                    /* that failed, try adding a new one */
                    if (!ll_insert(uservalues_ll, elem->name_value,
                        elem->name_value_len, (void*)val, sizeof(*val), 0)) {
                        free(val);
                        return CALCERR_VALUE_SET_FAILED;
                    }
                }
            }

            break;

        case ASSIGN_FUNCTION:
            for (size_t i = 0; i < elem->args_len; i++) {
                if (elem->args[i]->elem_type != ELEM_NAME) {
                    return CALCERR_ARG_TYPE_MISMATCH;
                }
            }

            struct FUNC* func = create_intr_func(elem);

            if (!ht_set(functions_ht, elem->name_value,
                        elem->name_value_len, func, NULL, FREE_ENTRY_TRUE)) {
                free_func(func);
                return CALCERR_VALUE_SET_FAILED;
            }

            *elem->child1->number_value = 0;
            break;
        }

        *elem->number_value = *elem->child1->number_value;
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

enum CALCERR save_ans()
{
    FILE* fp = NULL;

    fopen_s(&fp, "ans.bin", "wb");

    if (fp == NULL) {
        return CALCERR_ANS_SAVING_FAILED;
    }

    fwrite(&ans_len, sizeof(ans_len), 1, fp);
    fwrite(&ans_count, sizeof(ans_count), 1, fp);
    fwrite(ans_array, sizeof(double), ans_count, fp);

    fclose(fp);

    return CALCERR_NONE;
}

enum CALCERR load_ans()
{
    FILE* fp = NULL;

    fopen_s(&fp, "ans.bin", "rb");

    if (fp == NULL) {
        return CALCERR_ANS_LOADING_FAILED;
    }

    fread(&ans_len, sizeof(ans_len), 1, fp);
    fread(&ans_count, sizeof(ans_count), 1, fp);
    ans_array = malloc(ans_len * sizeof(double));
    fread(ans_array, sizeof(double), ans_count, fp);

    fclose(fp);

    return CALCERR_NONE;
}

void free_func_void(void* func_void)
{
    free_func((struct FUNC*)func_void);
}
