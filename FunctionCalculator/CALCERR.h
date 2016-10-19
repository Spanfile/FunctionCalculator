#pragma once

#define FOREACH_CALCERR(CALCERR)                        \
        CALCERR(CALCERR_NONE)                           \
        CALCERR(CALCERR_UNEXPECTED_TOKEN)               \
        CALCERR(CALCERR_EXPECTED_CLOSING_BRACKET)       \
        CALCERR(CALCERR_CONTAINER_MALLOC_ERROR)         \
        CALCERR(CALCERR_UNEXPECTED_END_OF_INPUT)        \
        CALCERR(CALCERR_INTERPRETER_INIT_FAILED)        \
        CALCERR(CALCERR_INTERPRETER_VALUE_SET_FAILED)   \
        CALCERR(CALCERR_DIVIDE_BY_ZERO)                 \
        CALCERR(CALCERR_INVALID_CHARACTER)              \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STR) #STR,

enum CALCERR {
    FOREACH_CALCERR(GENERATE_ENUM)
};

static const char* CALCERR_STRING[] = {
    FOREACH_CALCERR(GENERATE_STRING)
};