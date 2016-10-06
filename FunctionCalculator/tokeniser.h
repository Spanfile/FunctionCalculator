#pragma once // this is the same as having the #ifndef stuff
#include <string>

#include "TOKEN.h"
using namespace std;

TOKEN** tokenise(char*, size_t, size_t*);