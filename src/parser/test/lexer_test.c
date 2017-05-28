#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cmocka.h"

#include "../lexer.c"

void test_init_lexer_with_token_array() {
    Token **tok = (Token**) malloc(sizeof(Token*) * 5);
    init_lexer(tok);
    assert(tokens != NULL);
    free(tokens);
}

void setup(int tk_size) {
    Token **tok = (Token**) malloc(sizeof(*tok) * tk_size);
    init_lexer(tok);
}

void test_lex_number() {
    char *string = "123";
    setup(1);
    lex(string);
    assert(tokens[0]->type == NUMBER);
    assert_string_equal(tokens[0]->value, "123");
    free(tokens[0]);
    free(tokens);
}

void test_lex_identifier() {
    char *word = "foo";
    setup(1);
    lex(word);
    assert(tokens[0]->type == IDENTIFIER);
    assert_string_equal(tokens[0]->value, "foo");
    free(tokens[0]);
    free(tokens);
}

void test_lex_keyword() {
    char *string= "int";
    setup(1);
    lex(string);
    assert(tokens[0]->type == KEYWORD_int);
    assert_string_equal(tokens[0]->value, "int");
    free(tokens[0]);
    free(tokens);
}

void test_lex_statement() {
    char *string = "select 1 from foo";
    setup(4);
    lex(string);
    assert(tokens[0]->type == KEYWORD_select);
    assert_string_equal(tokens[0]->value, "select");
    assert(tokens[1]->type == NUMBER);
    assert_string_equal(tokens[1]->value, "1");
    assert(tokens[2]->type == KEYWORD_from);
    assert_string_equal(tokens[2]->value, "from");
    assert(tokens[3]->type == IDENTIFIER);
    assert_string_equal(tokens[3]->value, "foo");
    for (int i = 0; i<4; i++){
        free(tokens[i]);
    }
    free(tokens);
}

void test_lex_create_table() {
    char* string = "create table foo (a int, b text);";
    setup(20);
    lex(string);
    assert(tokens[3]->type == OPEN_PARENS);
    assert(tokens[6]->type == COMMA);
    assert(tokens[9]->type == CLOSED_PARENS);
    assert(tokens[10]->type == SEMICOLON);
    for (int i = 10; i > -1; i--) {
        free(tokens[i]);
    }
    free(tokens);
}

int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_init_lexer_with_token_array),
        cmocka_unit_test(test_lex_number),
        cmocka_unit_test(test_lex_identifier),
        cmocka_unit_test(test_lex_keyword),
        cmocka_unit_test(test_lex_statement),
        cmocka_unit_test(test_lex_create_table),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

