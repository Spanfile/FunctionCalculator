#ifndef _H_PARSER
#define _H_PARSER

#include <stdlib.h>

#include "TOKEN.h"
#include "tree_element.h"
#include "PRECEDENCE.h"
#include "PARSER_CONTAINER.h"
#include "parselets.h"
#include "CALCERR.h"

enum CALCERR create_parser_container(struct TOKEN** tokens, size_t token_count,
    int* index, struct PARSER_CONTAINER** container_out);
enum CALCERR parse(struct PARSER_CONTAINER* container, int precedence,
    struct TREE_ELEMENT** elem_out);

#endif
