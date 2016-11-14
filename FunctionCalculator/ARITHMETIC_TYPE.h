#pragma once

#define FOREACH_ARITHMETIC_TYPE(ARITH_TYPE)                                    \
    ARITH_TYPE(ARITH_ADDITION)                                                 \
    ARITH_TYPE(ARITH_NEGATION)                                                 \
    ARITH_TYPE(ARITH_MULTIPLICATION)                                           \
    ARITH_TYPE(ARITH_DIVISION)                                                 \
    ARITH_TYPE(ARITH_POWER)                                                    \
    ARITH_TYPE(ARITH_REMAINDER)

#ifndef GENERATE_ENUM
#define GENERATE_ENUM(ENUM) ENUM,
#endif
#ifndef GENERATE_STRING
#define GENERATE_STRING(STR) #STR,
#endif

enum ARITHMETIC_TYPE { FOREACH_ARITHMETIC_TYPE(GENERATE_ENUM) };

#pragma GCC diagnostic ignored "-Wunused-variable"
static const char* ARITHMETIC_TYPE_STRING[] = {
    FOREACH_ARITHMETIC_TYPE(GENERATE_STRING)};
