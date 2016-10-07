#pragma once

enum TOKENTYPE {
    NAME, NUMBER,
    ADDITION, NEGATION, MUILTIPLICATION, DIVISION, POWER, REMAINDER,
    OPEN_BRACKET, CLOSE_BRACKET
};

struct TOKEN {
    TOKENTYPE type;
    size_t value_length;
    char* value;
};
