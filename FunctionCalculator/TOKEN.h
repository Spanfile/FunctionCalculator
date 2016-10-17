#pragma once

enum TOKEN_TYPE {
    TOKEN_NAME, TOKEN_NUMBER,
    TOKEN_ADDITION, TOKEN_NEGATION, TOKEN_MULTIPLICATION, TOKEN_DIVISION,
    TOKEN_POWER, TOKEN_REMAINDER, TOKEN_OPEN_BRACKET, TOKEN_CLOSE_BRACKET
};

struct TOKEN {
    enum TOKEN_TYPE type;
    size_t value_length;
    char* value;
};
