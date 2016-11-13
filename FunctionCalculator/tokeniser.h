#pragma once // this is the same as having the #ifndef stuff

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TOKEN.h"
#include "CALCERR.h"

enum CALCERR tokenise(char*, size_t, size_t*, struct TOKEN***);
void free_tokens(struct TOKEN***, size_t);
