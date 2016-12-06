#pragma once // this is the same as having the #ifndef stuff

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TOKEN.h"
#include "CALCERR.h"

enum CALCERR tokenise(char* input, size_t input_len, size_t* token_count,
                      struct TOKEN*** tokens);
void free_tokens(struct TOKEN*** tokens, size_t token_count);
