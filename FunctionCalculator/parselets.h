#pragma once

#include "parser.h"
#include "PARSER_CONTAINER.h"
#include "tree_element.h"
#include "TOKEN.h"
#include "PRECEDENCE.h"
#include "CALCERR.h"

enum CALCERR parse_name(struct TOKEN*, struct PARSER_CONTAINER*, struct TREE_ELEMENT**);
enum CALCERR parse_number(struct TOKEN*, struct PARSER_CONTAINER*, struct TREE_ELEMENT**);
enum CALCERR parse_arithmetic(struct TOKEN*,
    struct TREE_ELEMENT*, struct PARSER_CONTAINER*, struct TREE_ELEMENT**);
enum CALCERR parse_group(struct TOKEN*, struct PARSER_CONTAINER*, struct TREE_ELEMENT**);
