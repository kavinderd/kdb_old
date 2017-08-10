#include "../utils/list.h"

#ifndef PARSER_H
#define PARSER_H
typedef enum {
    TERMINAL,
    EXPR,
    SELECT,
    SEL_LIST,
    SEL_LIST_MEMBER,
    FROM,
    REL_LIST,
    REL_LIST_MEMBER,
    CREATE_TABLE,
    REL_NAME,
    ATTR_LIST,
    ATTR_LIST_MEMBER

} ParseNodeType;
typedef struct {
    ParseNodeType type;
    List* children;
    char* value;
} ParseNode;
#endif

ParseNode* parse(char* string);
