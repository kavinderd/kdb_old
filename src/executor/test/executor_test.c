#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cmocka.h"
#include "../executor.c"
#include "../../planner/planner.h"
#include "../../utils/list.h"

void execute_scan(PlanState* node) {
    check_expected(node);
}

//TODO: Figure out how to properly unit test the planner
//This is hard because I need a ParseTree generated from a SQL query
QueryPlan* setup() {
    char* projected_column = "a";
    List* l = new_list();
    list_append(l, projected_column);
    QueryPlan *qp = (QueryPlan*) malloc(sizeof(*qp));
    Scan* scan_node = (Scan*) malloc(sizeof(*scan_node));
    scan_node->type = SCAN;
    scan_node->relation = "foo";
    scan_node->projection_list = l;
    qp->root = (Plan*) scan_node;
    return qp;
}

void test_initialize_plan() {
    QueryPlan* qp = setup();
    ExecutionState* es = (ExecutionState*) malloc(sizeof(*es));
    initialize_execution_state(qp, es);
    assert(es->status == READY);
    assert(es->plan->root->type == SCAN);
    assert(es->planstate->node->type == SCAN_STATE);
    assert(es->planstate->node == qp->root);
}

void test_execute_plan() {
    QueryPlan* qp = setup();
    ExecutionState* es = (ExecutionState*) malloc(sizeof(*es));
    initialize_execution_state(qp, es);
    expect_any(execute_scan, node);
    execute_plan(es->planstate);
}

int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_initialize_plan),
        cmocka_unit_test(test_execute_plan),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

