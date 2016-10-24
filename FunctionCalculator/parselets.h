#pragma once

#include "parser.h"
#include "PARSER_CONTAINER.h"
#include "tree_element.h"
#include "TOKEN.h"
#include "PRECEDENCE.h"
#include "CALCERR.h"
#include "pointer_help.h"

/* "prefix" parselets
    they take in their associated token, the parser container and a pointer to
    the element they should output to */
enum CALCERR parse_name(struct TOKEN*, struct PARSER_CONTAINER*, struct TREE_ELEMENT**);
enum CALCERR parse_number(struct TOKEN*, struct PARSER_CONTAINER*, struct TREE_ELEMENT**);
enum CALCERR parse_group(struct TOKEN*, struct PARSER_CONTAINER*, struct TREE_ELEMENT**);

/* "postfix" parselets
    they're otherwise the same as prefix parselets, except they take in the element 
    to their left */
enum CALCERR parse_arithmetic(struct TOKEN*, struct TREE_ELEMENT*,
    struct PARSER_CONTAINER*, struct TREE_ELEMENT**);
enum CALCERR parse_function(struct TOKEN*, struct TREE_ELEMENT*,
     struct PARSER_CONTAINER*, struct TREE_ELEMENT**);
