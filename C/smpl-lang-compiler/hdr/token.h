#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <regex.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_PROG_LINE_LEN
#define MAX_PROG_LINE_LEN 256
#endif//MAX_PROG_LINE_LEN

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
	char *raw_tkn;
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
init_token ();

TokenNode *
init_token_node (Token *t);

void
clear_token_node (TokenNode *tn);

void
push_token_node (TokenList *tl,
								 TokenNode *new_node);

void
print_token_list (TokenList *tl);

/* #define IDENT     re.compile(r"[a-zA-Z]([a-zA-Z0-9]+)?"); */
/* #define NUMBER    re.compile(r"[0-9]+"); */
/* #define OP_INEQ   "!="; */
/* #define OP_EQ     "=="; */
/* #define OP_LT     "<"; */
/* #define OP_LE     "<="; */
/* #define OP_GT     ">"; */
/* #define OP_GE     ">="; */
/* #define LBRACKET  "["; */
/* #define RBRACKET  "]"; */
/* #define LPAREN    "("; */
/* #define RPAREN    ")"; */
/* #define LBRACE    "{"; */
/* #define RBRACE    "}"; */
/* #define ASTERISK  "*"; */
/* #define SLASH     "/"; */
/* #define PLUS      "+"; */
/* #define MINUS     "-"; */
/* #define LET       "let"; */
/* #define LARROW    "<-"; */
/* #define CALL      "call"; */
/* #define IF        "if"; */
/* #define THEN      "then"; */
/* #define ELSE      "else"; */
/* #define FI        "fi"; */
/* #define WHILE     "while"; */
/* #define DO        "do"; */
/* #define OD        "od"; */
/* #define RETURN    "return"; */
/* #define VAR       "var"; */
/* #define ARRAY     "array"; */
/* #define SEMICOLON ";"; */
/* #define VOID      re.compile(r"void\s+"); */
/* #define FUNCTION  re.compile(r"function\s+"); */
/* #define MAIN      re.compile(r"main\s+"); */
/* #define PERIOD    "."; */
/* #define COMMA     ","; */

void
create_alnum_token (Token *t,
										char *buf,
										int line);

void
create_symbol_token (Token *t,
										 char *buf,
										 int line);

void
check_regex_compilation (int rv);

#endif//_TOKEN_H_
