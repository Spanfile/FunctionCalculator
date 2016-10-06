#pragma once // this is the same as having the #ifndef stuff

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TOKEN.h"

TOKEN** tokenise(char*, size_t, size_t*);
