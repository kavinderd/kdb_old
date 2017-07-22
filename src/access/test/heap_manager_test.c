#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "../heap_manager.c"

BufferPage *buffer_manager_get_page(char *rel_name, int page) {
    check_expected(rel_name);
    BufferPage *bp = (BufferPage *) malloc(sizeof(*bp));
    return bp;
}
void test_open_relation() {
    char *rel_name = "relation";
    expect_any(buffer_manager_get_page, rel_name);
    Relation *relation = heap_open_relation(rel_name);
}

/*
 * TODO: The setup for this test is a mess
 */
void test_build_tuple_descriptor() {
   TupleDescriptor *td = (TupleDescriptor *) malloc(sizeof(*td));
   td->attr_count = 1;
   AttributeData ad = {"i", INT, 32};
   //Set the attributes to NULL so that the don't contribute to the written data
   td->attrs = NULL;
   FILE *tmp = fopen("/tmp/tup_desc", "wb");
   fwrite(td, sizeof(td), 1, tmp);
   fwrite(&ad, sizeof(ad), 1, tmp);
   BufferPage *buf_page = (BufferPage *) malloc(sizeof(*buf_page)); 
   buf_page->rel_name = "test";
   buf_page->buffer = (char *) malloc(8192);
   fseek(tmp, 0, SEEK_SET);
   fclose(tmp);
   tmp = fopen("/tmp/tup_desc", "rb");
   fread(buf_page->buffer, sizeof(char), 8192, tmp);
   fclose(tmp);
   Relation *rel = (Relation *) malloc(sizeof(*rel));
   build_tuple_descriptor(rel, buf_page);
   assert_int_equal(rel->tuple_descriptor->attr_count, 1);
   assert_string_equal(rel->tuple_descriptor->attrs[0]->name, ad.name);
   assert_int_equal(rel->tuple_descriptor->attrs[0]->length, ad.length);
   remove("/tmp/tup_desc");
}

int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
//        cmocka_unit_test(test_open_relation),
        cmocka_unit_test(test_build_tuple_descriptor),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
