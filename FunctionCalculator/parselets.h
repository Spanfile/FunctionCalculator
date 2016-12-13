#ifndef _H_PARSELETS
#define _H_PARSELETS

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
enum CALCERR parse_name(struct TOKEN*  token, struct PARSER_CONTAINER* container,
                        struct TREE_ELEMENT** elem_out);
enum CALCERR parse_number(struct TOKEN* token, struct PARSER_CONTAINER* container,
                          struct TREE_ELEMENT** elem_out);
enum CALCERR parse_group(struct TOKEN* token, struct PARSER_CONTAINER* container,
                         struct TREE_ELEMENT** elem_out);
enum CALCERR parse_negation(struct TOKEN* token, struct PARSER_CONTAINER* container,
                            struct TREE_ELEMENT** elem_out);

/* "postfix" parselets
    they're otherwise the same as prefix parselets, except they take in the
    element to their left */
enum CALCERR parse_arithmetic(struct TOKEN* token, struct TREE_ELEMENT* left,
                              struct PARSER_CONTAINER* container, struct TREE_ELEMENT** elem_out);
enum CALCERR parse_function(struct TOKEN* token, struct TREE_ELEMENT* left,
                            struct PARSER_CONTAINER* container, struct TREE_ELEMENT** elem_out);
enum CALCERR parse_assignment(struct TOKEN* token, struct TREE_ELEMENT* left,
                              struct PARSER_CONTAINER* container, struct TREE_ELEMENT** elem_out);

#endif
