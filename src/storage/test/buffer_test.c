#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include "../buffer.c"

void setup() {
    for (int i = 0; i < BUFFER_POOL_SIZE; i++) {
        buffer_pool[i] = NULL;
    }
}

void test_get_free_buffer_page() {
    setup();
    BufferPage *bp = get_free_buffer_page();
    assert_non_null(bp);
}

void test_get_unreferenced_page() {
    setup();
    BufferPage *bp1 = (BufferPage *) malloc(sizeof(*bp1));
    bp1->ref_count = 0;
    buffer_pool[0] = bp1;
    BufferPage *bp = get_free_buffer_page();
    assert_ptr_equal(bp1, bp);
}

void test_skip_referenced_page() {
    setup();
    BufferPage *bp1 = (BufferPage *) malloc(sizeof(*bp1));
    bp1->rel_name = "test";
    bp1->ref_count = 1;
    BufferPage *bp2 = (BufferPage *) malloc(sizeof(*bp2));
    bp2->rel_name = "test2";
    bp2->ref_count = 0;
    buffer_pool[0] = bp1;
    buffer_pool[1] = bp2;
    BufferPage *bp = get_free_buffer_page();
    assert_ptr_equal(bp2, bp);
}


int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_free_buffer_page),
        cmocka_unit_test(test_get_unreferenced_page),
        cmocka_unit_test(test_skip_referenced_page),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
