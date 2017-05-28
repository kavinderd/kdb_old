#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include "../buffer_manager.c"

BufferPage *get_free_buffer_page() {
    function_called();
    return (BufferPage *) mock();
}

bool get_page_from_disk(char *buffer, char *rel_name, int page) {
    function_called();
    return true;
}

void test_get_page() {
    BufferPage *bp = (BufferPage *) malloc(sizeof(bp));
    will_return(get_free_buffer_page, bp);
    expect_function_call(get_free_buffer_page);
    expect_function_call(get_page_from_disk);
    buffer_manager_get_page("test", 0);
}

int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_page),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
