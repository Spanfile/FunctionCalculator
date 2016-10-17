#pragma once

#include "parser.h"
#include "PARSER_CONTAINER.h"
#include "tree_element.h"
#include "TOKEN.h"
#include "PRECEDENCE.h"

struct TREE_ELEMENT* parse_name(struct TOKEN*, struct PARSER_CONTAINER*);
struct TREE_ELEMENT* parse_number(struct TOKEN*, struct PARSER_CONTAINER*);
struct TREE_ELEMENT* parse_arithmetic(struct TOKEN*,
    struct TREE_ELEMENT*, struct PARSER_CONTAINER*);