#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cmocka.h"
#include "../scan_node.c"
#include "../../planner/planner.h"
#include "../../utils/list.h"

void open_relation(char* rel_name) {
    check_expected(rel_name);
}

void scan_next_tuple() {
    function_called();
}

ScanState* setup() {
    char* projected_column = "a";
    List* l = new_list();
    list_append(l, projected_column);
    Scan* scan_node = (Scan*) malloc(sizeof(*scan_node));
    scan_node->type = SCAN;
    scan_node->relation = "foo";
    scan_node->projection_list = l;
    ScanState *ss = (ScanState*) malloc(sizeof(*ss));
    ss->type = SCAN_STATE;
    ss->node = (Plan*) scan_node;
    ss->rel_name = scan_node->relation;
    ss->projection_list = scan_node->projection_list;
    ss->status = UNINITIALIZED;
    return ss;
}

void test_initialize_scan() {
    ScanState *ss = setup();
    expect_any(open_relation, rel_name);
    initialize_scan(ss);
}

void test_execute_scan() {
    ScanState *ss = setup();
    ss->status = READY;
    expect_function_call(scan_next_tuple);
    execute_scan(ss);
}

int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_initialize_scan),
        cmocka_unit_test(test_execute_scan),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
