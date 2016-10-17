#pragma once

#include "parser.h"
#include "PARSER_CONTAINER.h"
#include "tree_element.h"
#include "TOKEN.h"
#include "PRECEDENCE.h"

TREE_ELEMENT* parse_name(TOKEN*, PARSER_CONTAINER*);
TREE_ELEMENT* parse_number(TOKEN*, PARSER_CONTAINER*);
TREE_ELEMENT* parse_arithmetic(TOKEN*, TREE_ELEMENT*, PARSER_CONTAINER*);