/*
 * KDB parser
 *
 * This parser takes the following approach to parsing a SQL statement:
 *
 * - First calles the lexer to generate all the tokens of a string
 * - Then constructs a parse tree of tokens
 */

#include <stdlib.h>
#include <stdio.h>
#include "../utils/list.h"
#include "parser.h"
#include "lexer.h"



int generate_list_subtree(Token** tokens, int index, ParseNode* root, ParseNodeType type) {
    //SEL_LIST node
    ParseNodeType list_member;
    TokenType terminating_type;
    switch(type) {
        case SEL_LIST:
            list_member = SEL_LIST_MEMBER;
            terminating_type = KEYWORD_from;
            break;
        case REL_LIST:
            list_member = REL_LIST_MEMBER;
            terminating_type = KEYWORD_where;
            break;
        case ATTR_LIST:
            list_member = ATTR_LIST_MEMBER;
            terminating_type = CLOSED_PARENS;
        default:
            break;
    }
    ParseNode* list_node = (ParseNode*) malloc(sizeof(*list_node));
    list_node->type = type;
    list_node->children = new_list();
    while (tokens[index]->type != terminating_type && tokens[index]->type != SEMICOLON) {
        if (tokens[index]->type == COMMA) {
            index++;
            continue;
        }

        ParseNode* node = (ParseNode*) malloc(sizeof(*node));
        node->type = list_member;
        if (list_member == ATTR_LIST_MEMBER) {
            //TODO: Special case. This needs to be fixed. ParseNode should support multiple values
            node->children = new_list();
            ParseNode* attr_name = (ParseNode*) malloc(sizeof(*attr_name));
            attr_name->value = tokens[index++]->value;
            list_append(node->children, attr_name);
            ParseNode* attr_type = (ParseNode*) malloc(sizeof(*attr_type));
            attr_type->value = tokens[index]->value;
            list_append(node->children, attr_type);
            list_append(list_node->children, node);
            index++;
        } else {
            node->value = tokens[index]->value;
            list_append(list_node->children, node);
            index++;
        }
    }
    list_append(root->children, list_node);
    return index;
}

/*
 * Generate Select ParseTree
 *
 * We know we are forming a select statement's parsetree so iterate
 * over the tokens in the appropriate fashion and insert any addtional
 * ParseNodes appropriately
 *
 * A select statement will have a parse tree of the following form:
 *
 *                              EXPR
 *
 *          SELECT SEL_LIST  FROM   REL_LIST   WHERE QUAL_LIST
 *
 *              <LIST of sels>     <LIST of rels>    <LIST of quals>
 */
void generate_select_parsetree(Token** tokens, ParseNode* root) {
    int tok_idx = 0;
    int i = 0;
    root->children = new_list();
    
    //SELECT node
    ParseNode* select_node = (ParseNode*) malloc(sizeof(*select_node));
    select_node->type = SELECT;
    list_append(root->children, select_node);
    i++;

    i = generate_list_subtree(tokens, i, root, SEL_LIST);

    if (tokens[i]->type == SEMICOLON) {
        return;
    }

    //FROM node
    ParseNode* from_node = (ParseNode*) malloc(sizeof(*from_node));
    from_node->type = FROM;
    list_append(root->children, from_node);
    i++;

    i = generate_list_subtree(tokens, i, root, REL_LIST);
    //Leaving out WHERE qualifiers for now
}

void generate_create_table_parsetree(Token** tokens, ParseNode* root) {
    //TODO: IMPLEMENT
    int tok_idx = 0;
    int i = 0;
    root->children = new_list();

    //CREATE TABLE Node
    ParseNode* create_table_node = (ParseNode*) malloc(sizeof(*create_table_node));
    create_table_node->type = CREATE_TABLE;
    list_append(root->children, create_table_node); 
    //add 2 to index to account for "create" and "table" tokens
    i+=2;

    //Rel Name Node
    ParseNode* table_name = (ParseNode*) malloc(sizeof(*table_name));
    table_name->type = REL_NAME;
    table_name->value = tokens[i]->value;
    list_append(root->children, table_name);
    i++;

    i++; //Skip the open parens
    i = generate_list_subtree(tokens, i, root, ATTR_LIST);
}

void generate_create_parsetree(Token** tokens, ParseNode* root) {
    //Figure out what the next token is 'table, index, database, etc'
    int tok_idx = 1;
    switch(tokens[tok_idx]->type) {
        case KEYWORD_table:
            generate_create_table_parsetree(tokens, root);
            break;
        default:
            break;
    }
}

/*
 * This function takes the following steps:
 * 1. Figure out the first token in the statement:
 *     'select...'
 *     'create...'
 *    etc
 * 2. Once that is known, call the corresponding function 
 *    to generate the appropriate parsetree
 */
void generate_parsetree(Token** tokens, ParseNode* root) {
    switch(tokens[0]->type) {
        case KEYWORD_select:
            generate_select_parsetree(tokens, root);
            break;
        case KEYWORD_create:
            generate_create_parsetree(tokens, root);
            break;
        case KEYWORD_insert:
        case KEYWORD_drop:
        default:
            break;
    }
}

ParseNode* parse(char* string) {
    Token **tok;
    init_lexer(tok);
    tok = lex(string);
    ParseNode* root = malloc(sizeof(*root));
    root->type = EXPR;
    int i = 0;
    root->children = new_list();
    generate_parsetree(tok, root);
    return root;
}
