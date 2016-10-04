#pragma once
#include <string>
using namespace std;

typedef enum {
    Name, Number, Arithmetic, OpenBracket, CloseBracket
} TOKENTYPE;

typedef struct {
    TOKENTYPE type;
    string value;
} TOKEN;