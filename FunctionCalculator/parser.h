#pragma once

#include <stdlib.h>

#include "TOKEN.h"
#include "TREE_ELEMENT.h"
#include "PRECEDENCE.h"
#include "PARSER_CONTAINER.h"
#include "parselets.h"

PARSER_CONTAINER* create_parser_container(TOKEN**, size_t, int*);
TREE_ELEMENT* parse(PARSER_CONTAINER*, int precedence);