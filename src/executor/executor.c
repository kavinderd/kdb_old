#include <stdlib.h>
#include "executor.h"

extern void execute_scan(PlanState* node);

void initialize_execution_node(Plan* node, ExecutionState* execution_state) {
    execution_state->planstate = (PlanState*) malloc(sizeof(*(execution_state->planstate)));
    execution_state->planstate->node= node;
    PlanStateType t;
    switch(node->type) {
        case SCAN:
            t = SCAN_STATE;
            break;
        default:
            break;
    }
    execution_state->planstate->type = t;
}

void initialize_execution_state(QueryPlan* query_plan, ExecutionState* execution_state) {
    execution_state->status = READY;
    execution_state->plan = query_plan;
    initialize_execution_node(query_plan->root, execution_state);
}

void execute_plan(PlanState* plan_state) {
    switch(plan_state->type) {
        case SCAN_STATE:
            execute_scan(plan_state);
            break;
        default:
            break;
    }
}
