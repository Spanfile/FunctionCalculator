#pragma once

#include <stdlib.h>

#include "TOKEN.h"
#include "tree_element.h"
#include "PRECEDENCE.h"
#include "PARSER_CONTAINER.h"
#include "parselets.h"

struct PARSER_CONTAINER* create_parser_container(struct TOKEN**, size_t, int*);
struct TREE_ELEMENT* parse(struct PARSER_CONTAINER*, int precedence);