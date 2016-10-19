#pragma once

#include <stdlib.h>

#include "TOKEN.h"
#include "tree_element.h"
#include "PRECEDENCE.h"
#include "PARSER_CONTAINER.h"
#include "parselets.h"
#include "CALCERR.h"

enum CALCERR create_parser_container(struct TOKEN**, size_t, int*,
    struct PARSER_CONTAINER**);
enum CALCERR parse(struct PARSER_CONTAINER*, int precedence,
    struct TREE_ELEMENT**);