#pragma once

#include <math.h>
#include <stdio.h>

#include "tree_element.h"
#include "hashtable.h"
#include "CALCERR.h"
#include "pointer_help.h"
#include "func.h"

enum CALCERR init_interpreter(void);
enum CALCERR free_interpreter(void);
// extra_names is used for function calls
// user values are added to the main name hashtable
enum CALCERR evaluate_element(struct TREE_ELEMENT*, struct HASHTABLE*);