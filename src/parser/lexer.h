/*
 * Header file for lexer.c
 */
#ifndef LEXER_H
#define LEXER_H
typedef enum TokenType {
    NUMBER,
    IDENTIFIER,
    KEYWORD,
    OPEN_PARENS,
    CLOSED_PARENS,
    SEMICOLON,
    COMMA,
    KEYWORD_select,
    KEYWORD_create,
    KEYWORD_drop,
    KEYWORD_insert,
    KEYWORD_table,
    KEYWORD_where,
    KEYWORD_from,
    KEYWORD_int,
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;
#endif
extern int position;
extern int token_index;
extern Token** tokens;

extern void init_lexer(Token** tk);
extern Token** lex(char* string);
