#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cmocka.h"
#include "../server.c"

ParseNode* parse(char *string) {
    function_called();
    return NULL;
}

QueryPlan* generate_plan(ParseNode *pn){
    function_called();
    return NULL;
}

void initialize_execution_state(QueryPlan *qp, ExecutionState *estate) {
    function_called();
}

void execute_plan(ExecutionState *ps) {
    function_called();
}

void test_server_execute_query() {

    /* BufferPage *bp = (BufferPage *) malloc(sizeof(bp)); */
    /* will_return(get_free_buffer_page, bp); */
    /* expect_function_call(get_free_buffer_page); */
    /* expect_function_call(get_page_from_disk); */
    /* buffer_manager_get_page("test", 0); */
    expect_function_call(parse);
    expect_function_call(generate_plan);
    expect_function_call(initialize_execution_state);
    expect_function_call(execute_plan);
    execute_query("select * from foo");
}

int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_server_execute_query),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
