/*
 * Planner
 *
 * The planner takes a ParseTree as input and generates a QueryPlan as output
 *
 * Currently there is no optimization done and the rules for queryplanning are very static and crude in nature
 */


#include <assert.h>
#include <stdlib.h>

#include "../parser/parser.h"
#include "planner.h"

/*
 * Generate a simple plan for the following query
 *
 * select a,b..n, from foo;
 */

char* extract_relation(List* parse_nodes) {
   ListItem* current = parse_nodes->head;
   while(((ParseNode*) current->ptr)->type != REL_LIST) {
       current = current->next;
   }
   return ((ParseNode*)((ParseNode*)current->ptr)->children->head->ptr)->value;
}

List* extract_projection_list(List* parse_nodes) {
   ListItem* current = parse_nodes->head;
   while(((ParseNode*) current->ptr)->type != SEL_LIST) {
       current = current->next;
   }
   current = ((ParseNode*)current->ptr)->children->head;
   List* list = new_list();
   while(current != NULL) {
       list_append(list, ((ParseNode*)current->ptr)->value);
       current = current->next;
   }
   return list;
}

QueryPlan* generate_select_plan(List* parse_nodes) {
    QueryPlan* plan = (QueryPlan*) malloc(sizeof(*plan));
    char *rel = extract_relation(parse_nodes);
    List* proj_list = extract_projection_list(parse_nodes);
    Scan* scan_node = (Scan*) malloc(sizeof(*scan_node));
    scan_node->type = SCAN;
    scan_node->relation = rel;
    scan_node->projection_list = proj_list;
    plan->root = (Plan*) scan_node;
    return plan;
}

QueryPlan* generate_plan(ParseNode* node) {
    if(node->type == EXPR) {
        assert(node->children->length >= 4);
        if (((ParseNode*) node->children->head->ptr)->type == SELECT) {
            return generate_select_plan(node->children);
        }
    }
    return NULL;
}
