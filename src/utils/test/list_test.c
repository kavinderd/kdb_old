#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cmocka.h"
#include "../list.c"

void test_new_list() {
    List* list = new_list();
    assert(list != NULL);
    assert(list->head == NULL);
    assert(list->tail == NULL);
    assert(list->length == 0);
    free(list);
}

void test_list_append() {
    List* list = new_list();
    int p= 5;
    list_append(list, &p);
    assert(list->length == 1);
    assert(list->head->ptr == &p);
    list_append(list, &p);
    assert(list->length == 2);
    assert(list->tail->ptr == &p);
}

void test_list_remove() {
    List *list = new_list();
    int a = 10;
    list_append(list, &a);
    assert(list->length == 1);
    int b = 15;
    list_append(list, &b);
    assert(list->length == 2);
    list_remove(list, &a);
    assert(list->length == 1);
    list_remove(list, &b);
    assert(list->length == 0);
    free(list);
}

int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_new_list),
        cmocka_unit_test(test_list_append),
        cmocka_unit_test(test_list_remove),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

