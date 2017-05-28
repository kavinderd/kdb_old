#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include "../storage_manager.c"

void test_get_page_from_disk() {
    //TODO: Clean up this test setup, it's gross
    char *relation = "test";
    char *env = getenv("KDB_HOME");
    setenv("KDB_HOME", "tmp", 1);
    char *path = "/tmp/test";
    char *buf = "test data";
    FILE *fd = fopen(path, "w+");
    fwrite(buf, sizeof(char), 10, fd);
    fclose(fd);
    char *buffer = (char *) malloc(8192);
    get_page_from_disk(buffer, relation, 0);
    char result[10];
    strncpy(result, buffer, 10);
    assert_string_equal(buf, result);
    free(buffer);
}

void test_get_non_existent_rel_from_disk () {
    char *buffer = (char *) malloc(8192);
    bool ret = get_page_from_disk(buffer, "no_rel", 0);
    assert_false(ret);
}

void test_get_non_zero_page_from_disk() {
    char *relation = "test";
    char *env = getenv("KDB_HOME");
    setenv("KDB_HOME", "tmp", 1);
    char *path = "/tmp/test";
    char *buf = "test data";
    FILE *fd = fopen(path, "wb");
    char arr[8192];
    fwrite(arr, sizeof(char), 8192, fd);
    char *page_2_content = "page 2";
    fwrite(page_2_content, sizeof(char), 7, fd);
    fclose(fd);
    char *buffer = (char *) malloc(8192);
    get_page_from_disk(buffer, relation, 1);
    char result[7];
    strncpy(result, buffer, 7);
    assert_string_equal(page_2_content, result);
}

int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_page_from_disk),
        cmocka_unit_test(test_get_non_existent_rel_from_disk),
        cmocka_unit_test(test_get_non_zero_page_from_disk),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
