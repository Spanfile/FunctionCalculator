#pragma once

#define FOREACH_CALCERR(CALCERR)                                               \
    CALCERR(CALCERR_NONE)                                                      \
    CALCERR(CALCERR_MALLOC_FAILED)                                             \
    CALCERR(CALCERR_REALLOC_FAILED)                                            \
    CALCERR(CALCERR_INTR_INIT_FAILED)                                          \
    CALCERR(CALCERR_INTR_VALUE_SET_FAILED)                                     \
    CALCERR(CALCERR_INTR_ELEMENT_NOT_IMPLEMENTED)                              \
    CALCERR(CALCERR_UNEXPECTED_TOKEN)                                          \
    CALCERR(CALCERR_EXPECTED_CLOSING_BRACKET)                                  \
    CALCERR(CALCERR_UNEXPECTED_END_OF_INPUT)                                   \
    CALCERR(CALCERR_DIVIDE_BY_ZERO)                                            \
    CALCERR(CALCERR_INVALID_CHARACTER)                                         \
    CALCERR(CALCERR_INVALID_ELEMENT)                                           \
    CALCERR(CALCERR_NAME_NOT_FOUND)                                            \
    CALCERR(CALCERR_ARG_COUNT_MISMATCH)                                        \
    CALCERR(CALCERR_ARG_TYPE_MISMATCH)                                         \
    CALCERR(CALCERR_ARG_CONVERSION_FAILED)

#ifndef GENERATE_ENUM
#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STR) #STR,
#endif

enum CALCERR { FOREACH_CALCERR(GENERATE_ENUM) };

#pragma GCC diagnostic ignored "-Wunused-variable"
static const char* CALCERR_STRING[] = {FOREACH_CALCERR(GENERATE_STRING)};