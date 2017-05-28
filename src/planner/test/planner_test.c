#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cmocka.h"
#include "../planner.c"
#include "../../parser/parser.h"


//TODO: Figure out how to properly unit test the planner
//This is hard because I need a ParseTree generated from a SQL query
ParseNode* setup() {
    char* string = "select a from foo;";
    ParseNode* node = parse(string);
    assert(node->children->length == 4);
    return node;
}
void test_generate_plan() {
    ParseNode* node = setup();
    assert(node->children->length == 4);
    QueryPlan* qp = generate_plan(node);
    assert_non_null(qp);
    assert_non_null(qp->root);
    Scan* scan = (Scan*) qp->root;
    assert(scan->type == SCAN);
    assert_string_equal(scan->relation, "foo");
    assert_string_equal(((char*) scan->projection_list->head->ptr), "a");
    free(node);
}

int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_generate_plan),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

