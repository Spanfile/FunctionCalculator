#pragma once

#include "PARSER_CONTAINER.h"
#include "TREE_ELEMENT.h"
#include "TOKEN.h"

TREE_ELEMENT* parse_name(TOKEN*, PARSER_CONTAINER*);
TREE_ELEMENT* parse_number(TOKEN*, PARSER_CONTAINER*);
TREE_ELEMENT* parse_arithmetic(TOKEN*, TREE_ELEMENT*, PARSER_CONTAINER*);