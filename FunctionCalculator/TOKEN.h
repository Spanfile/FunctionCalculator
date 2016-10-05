#pragma once
#include <string>
using namespace std;

enum TOKENTYPE {
    Name, Number, Arithmetic, OpenBracket, CloseBracket
};

struct TOKEN {
    TOKENTYPE type;
    char value[16]; /* there's 16 digits in the theoretical max value
                    for a double so that will have to do for us */
};