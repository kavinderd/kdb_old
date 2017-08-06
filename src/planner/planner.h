#include "../utils/list.h"
#include "../parser/parser.h"
#ifndef PLANNER_H
#define PLANNER_H
typedef enum {
    SCAN,
    AGG,
    JOIN
} NodeType;

typedef struct {
    NodeType type;
} Plan;

typedef struct {
    NodeType type;
    char* relation;
    List* projection_list;
} Scan;

typedef struct {
    Plan* root; 
} QueryPlan;

extern QueryPlan* generate_plan(ParseNode *pn);
#endif
