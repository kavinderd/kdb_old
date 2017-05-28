/*
 * In parsing, the lexer is responsible for taking an input string and generating a set of tokens.
 * These tokens are then passed to the parser which is able to then generate a syntax tree or call
 * the appropriate functions.
 *
 * This lexer implements the following algorithms:
 *
 * 1. Initialize a series of global variable to track our position in the string
 * 2. Iterate from the start of the string
 * 3. Pass each char through a switch statement to match against all available chars
 * 4. When encountering an alphanumeric string, first check if it is a keyword then assume it's an identifier
 * 5. Generate tokens for each 'object' (KEYWORDS, IDENTIFIERS, TERMINATING CHARS, ETC)
 * 6. When identifying multi-char TOKENS, when you find a char that could start that token gather as many chars
 * as could qualify for that token and then detect if that token is of a certain type.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"

#define MAX_SIZE 50
const char *KEYWORDS[]  = {"select", "create", "insert", "drop", "delete", "table", "from", "where", "int", "text", "init"};
static const int keyword_count = 11;

int position;
int token_index;
Token** tokens;

int tokenize_number(int position, char *string);
int tokenize_identifier_or_keyword(int position, char *string);
void add_token(TokenType token, char* string);

void init_lexer(Token** tk) {
    position = 0;
    tokens = (Token**) malloc(sizeof(*tokens) * MAX_SIZE);
    token_index = 0;
}

Token** lex(char *string) {
    char current;
    while (position < strlen(string)) {
        current =string[position];
        switch(current) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                position = tokenize_number(position, string);
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
                position = tokenize_identifier_or_keyword(position, string);
                break;
            case ' ':
                position++;
                break;
            case '(':
                add_token(OPEN_PARENS, "(");
                position++;
                break;
            case ')':
                add_token(CLOSED_PARENS, ")");
                position++;
                break;
            case ',':
                add_token(COMMA, ",");
                position++;
                break;
            case ';':
                add_token(SEMICOLON, ";");
                position++;
                break;
            default:
                break;
        }
    }
    return tokens;
}

void add_token(TokenType type, char *string) {
    tokens[token_index]= (Token *) malloc(sizeof(**tokens));
    tokens[token_index]->type  = type;
    tokens[token_index]->value = string;
    token_index++;
}

int extract_substring_for_type(TokenType t, int position, char* string, char **dest) {
   int i = position;
    switch(t) {
        case NUMBER: {
            while (string[i] >= '0' && string[i] <= '9')
                i++;
            }
            break;
        case KEYWORD:
        case IDENTIFIER: {
            while ((string[i] >= 'a' && string[i] <= 'z') || (string[i] >= '0' && string[i] <= '9') || string[i] == '_')
                i++;
            }
            break;
        default:
            break;
    }
    *dest = (char *)malloc(sizeof(char) * (i - position));
    int j,index;
    for (j = 0, index=position; j < (i - position); j++, index++) {
        (*dest)[j] = string[index];
    }
    (*dest)[++j] = '\0';
    return i;
}

int tokenize_number(int position,char* string) {
    char* substring;
    int new_pos = extract_substring_for_type(NUMBER, position, string, &substring);
    add_token(NUMBER, substring);
    return new_pos;
}

TokenType get_type_for_string(char* string) {
    TokenType t = IDENTIFIER;
    if (strcmp(string, "select") == 0) {
        t = KEYWORD_select;
    } else if (strcmp(string, "create") == 0){
        t = KEYWORD_create;
    } else if (strcmp(string, "from") == 0) {
        t = KEYWORD_from;
    } else if (strcmp(string, "drop") == 0) {
        t = KEYWORD_drop;
    } else if (strcmp(string, "int") == 0) {
        t = KEYWORD_int;
    }
    return t;
}
int tokenize_identifier_or_keyword(int position, char *string) {
    char *substring;
    int new_pos = extract_substring_for_type(IDENTIFIER, position, string, &substring);
    TokenType t = get_type_for_string(substring);
    add_token(t, substring);
    return new_pos;
}


