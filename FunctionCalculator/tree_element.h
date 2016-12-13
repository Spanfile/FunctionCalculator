#ifndef _H_TREE_ELEMENT
#define _H_TREE_ELEMENT

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "ARITHMETIC_TYPE.h"

#define FOREACH_ELEM_TYPE(ELEM_TYPE)                                           \
    ELEM_TYPE(ELEM_NUMBER)                                                     \
    ELEM_TYPE(ELEM_NEGATION)                                                   \
    ELEM_TYPE(ELEM_ARITHMETIC)                                                 \
    ELEM_TYPE(ELEM_NAME)                                                       \
    ELEM_TYPE(ELEM_FUNCTION)                                                   \
    ELEM_TYPE(ELEM_ASSIGNMENT)

#define FOREACH_ASSIGNMENT_TYPE(ASSIGN_TYPE)                                   \
    ASSIGN_TYPE(ASSIGN_NAME)                                                   \
    ASSIGN_TYPE(ASSIGN_FUNCTION)

#ifndef GENERATE_ENUM
#define GENERATE_ENUM(ENUM) ENUM,
#endif
#ifndef GENERATE_STRING
#define GENERATE_STRING(STR) #STR,
#endif

enum TREE_ELEMENT_TYPE { FOREACH_ELEM_TYPE(GENERATE_ENUM) };
enum ASSIGNMENT_TYPE { FOREACH_ASSIGNMENT_TYPE(GENERATE_ENUM) };

#pragma GCC diagnostic ignored "-Wunused-variable"
static const char* TREE_ELEMENT_TYPE_STRING[] = {
    FOREACH_ELEM_TYPE(GENERATE_STRING)};

#pragma GCC diagnostic ignored "-Wunused-variable"
static const char* ASSIGNMENT_TYPE_STRING[] = {
    FOREACH_ASSIGNMENT_TYPE(GENERATE_STRING)};

struct TREE_ELEMENT {
    enum TREE_ELEMENT_TYPE elem_type;

    /* for ELEM_ARITHMETIC */
    enum ARITHMETIC_TYPE arithmetic_type;

    /* for ELEM_ASSIGNMENT */
    enum ASSIGNMENT_TYPE assign_type;

    /* for ELEM_NUMBER */
    int free_number_value;
    double* number_value;

    /* for ELEM_NAME and ELEM_FUNCTION */
    size_t name_value_len;
    char* name_value;

    /* for ELEM_FUNCTION */
    struct TREE_ELEMENT** args;
    size_t args_len;

    struct TREE_ELEMENT* child1;
    struct TREE_ELEMENT* child2;
};

struct TREE_ELEMENT* create_element(enum TREE_ELEMENT_TYPE type);
struct TREE_ELEMENT* create_number_element(double* value);
struct TREE_ELEMENT* create_negation_element(void);
struct TREE_ELEMENT* create_arithmetic_element(enum ARITHMETIC_TYPE type);
struct TREE_ELEMENT* create_name_element(char* value, size_t value_len);
struct TREE_ELEMENT* create_function_element(char* value, size_t value_len);
struct TREE_ELEMENT* create_assignment_element(char* value, size_t value_len,
                                               enum ASSIGNMENT_TYPE assign_type);

void copy_elem(struct TREE_ELEMENT* dst, struct TREE_ELEMENT* src);
void free_elem(struct TREE_ELEMENT* elem);

#endif
