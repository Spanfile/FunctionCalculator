#pragma once

#include <math.h>
#include <stdio.h>

#include "tree_element.h"
#include "hashtable.h"

void init_interpreter(void);
double interpret(struct TREE_ELEMENT*);