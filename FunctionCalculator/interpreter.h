#pragma once

#include <math.h>
#include <stdio.h>

#include "tree_element.h"
#include "hashtable.h"
#include "CALCERR.h"

enum CALCERR init_interpreter(void);
enum CALCERR evaluate_element(struct TREE_ELEMENT*);