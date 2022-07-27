#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_TKN_LEN
#define MAX_TKN_LEN 64
#endif//MAX_TKN_LEN

typedef enum TokenType {
	IDENT,
	NUMBER,
	OP_INEQ,
	OP_EQ,
	OP_LT,
	OP_LE,
	OP_GT,
	OP_GE,
	LBRACKET,
	RBRACKET,
	LPAREN,
	RPAREN,
	LBRACE,
	RBRACE,
	ASTERISK,
	SLASH,
	PLUS,
	MINUS,
	LET,
	LARROW,
	CALL,
	IF,
	THEN,
	ELSE,
	FI,
	WHILE,
	DO,
	OD,
	RETURN,
	VAR,
	ARRAY,
	SEMICOLON,
	VOID,
	FUNCTION,
	MAIN,
	PERIOD,
	COMMA,
} TokenType;

typedef struct Token {
	TokenType type;
	char *raw;// raw token string
	int val;  // just for #s, left unused for others
	int line; // line # on which it was parsed
} Token;

typedef struct TokenNode {
	Token *tkn;
	struct TokenNode *next;
} TokenNode;

typedef struct TokenList {
	TokenNode *head;
} TokenList;

Token *
new_token ();

TokenNode *
new_token_node (Token *t);

TokenList *
new_token_list ();

void
next_token (TokenNode **tn);

TokenNode *
push_token (TokenList *tl,
						Token *t);

void
print_token_list (TokenList *tl);

void
free_token_list (TokenList *tl);

Token *
new_alnum_token (char *buf,
								 int line);

Token *
new_symbol_token (char *buf,
									int line);

void
check_regex_compilation (int rv);

#endif//_TOKEN_H_
