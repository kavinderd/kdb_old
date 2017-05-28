#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "cmocka.h"
#include "../lexer.h"
#include "../parser.c"

//TODO: Write real unit tests on the individual functions
extern void init_lexer(Token **tk);
extern Token** lex(char* string);
void init_lexer(Token **tk) {
}

Token** lex(char* string) {
    return (Token**)((unsigned long long)mock());
}
//TODO: Remove assert() with proper assert function
void test_select_1() {
    char *string = "select 1";
    static Token select_tok = { KEYWORD_select, "select"};
    static Token number_tok = { NUMBER, "1"};
    static Token semicolon_tok = { SEMICOLON, ";"};
    Token** stub = malloc(sizeof(*stub) * 3);
    stub[0] = &select_tok;
    stub[1] = &number_tok;
    stub[2] = &semicolon_tok;
    will_return_count(lex, stub, 1);
    ParseNode* node = parse(string);
    assert(node != NULL);
    assert(node->type == EXPR);
    assert(node->children);
    assert(node->children->length == 2);
    assert(((ParseNode*) node->children->tail->ptr)->type == SEL_LIST);
    assert(((ParseNode*) ( (ListItem*) node->children->head)->ptr)->type == SELECT);
    free(stub);
}

void test_complex_select() {
    char *string = "select a, b, c from foo;";
    static Token select_tok = { KEYWORD_select, "select"};
    static Token col_a = { IDENTIFIER, "a"};
    static Token comma = { COMMA, ","};
    static Token col_b = { IDENTIFIER, "b"};
    static Token comma_2 = { COMMA, ","};
    static Token col_c = { IDENTIFIER, "c"};
    static Token from = { KEYWORD_from, "from"};
    static Token table = { IDENTIFIER, "foo"};
    static Token semicolon = { SEMICOLON, ";"};
    Token* stub[] = {&select_tok, &col_a, &comma, &col_b, &comma_2, &col_c, &from, &table, &semicolon};
    will_return_count(lex, stub, 1);
    ParseNode* node = parse(string);
    assert(node != NULL);
    assert(node->type == EXPR);
    assert(node->children);
    assert(node->children->length == 4);
    assert(((ParseNode*) node->children->tail->ptr)->type == REL_LIST);
    assert(((ParseNode*) ( (ListItem*) node->children->head)->ptr)->type == SELECT);
    ParseNode* sel_list = (ParseNode*) node->children->head->next->ptr;
    assert(sel_list->children->length == 3);
    assert_string_equal(((ParseNode*)sel_list->children->head->ptr)->value, "a");
    assert_string_equal(((ParseNode*)sel_list->children->tail->ptr)->value, "c");
    
    //REL_LIST test
    ParseNode* rel_list = (ParseNode*) node->children->tail->ptr;
    assert_int_equal(rel_list->children->length, 1);
    assert_string_equal(((ParseNode*)rel_list->children->head->ptr)->value, "foo");
}

int main(int argc, char* argv[]) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_select_1),
        cmocka_unit_test(test_complex_select)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
