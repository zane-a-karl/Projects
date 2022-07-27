/**
 * The EBNF for the respective morpheme is located above each
 * function sharing the same name as the morpheme. I include
 * the "alphabet"-like terms here for convenience
 *  as they have already been taken into account during lexing.
 * letter = “a”|“b”|...|“z”.
 * digit = “0”|“1”|...|“9”.
 * relOp = “==“|“!=“|“<“|“<=“|“>“|“>=“.
 * ident = letter {letter | digit}.
 * number = digit {digit}.
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include "../hdr/utils.h"
#include "../hdr/token.h"
#include "../hdr/lexer.h"
#include "../hdr/ast.h"

//#include "../hdr/var_table.h"
#include "../hdr/var_decl_list.h"
#include "../hdr/func_decl_list.h"
#include "../hdr/stmt_list.h"
#include "../hdr/computation.h"

//#include "../hdr/basic_block.h"
//#include "../hdr/dlx.h"
//#include "../hdr/result_list.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* static const int NUM_REGISTERS = 1<<5; //32 */
/* static const int NUM_OPCODES = 1<<6; //64 */
#ifndef MAX_NUM_VARS
#define MAX_NUM_VARS 1<<8
#endif//MAX_NUM_VARS

#ifndef MAX_NAME_LEN
#define MAX_NAME_LEN 1<<4
#endif//MAX_NAME_LEN

#ifndef MAX_LINE_LEN
#define MAX_LINE_LEN 1<<8
#endif//MAX_LINE_LEN

typedef enum ParserError {
	COMPUTATION_NO_LBRACE,// Cannot open main
	COMPUTATION_NO_RBRACE,// Cannot close main
	COMPUTATION_NO_MAIN  ,// No beginning 'main'
	COMPUTATION_NO_PERIOD,// No ending period

	VAR_DECL_NO_VAR_NAME ,// Cannnot find var name
	VAR_DECL_NO_COMMA    ,// No separating comma	
	VAR_DECL_NO_SEMICOLON,// No ending semicolon

	TYPE_DECL_NO_VAR       ,// No prefixed var keyword
	TYPE_DECL_NO_ARRAY     ,// No prefixed array keyword	
	TYPE_DECL_NO_LBRACKET  ,// No opening array bracket
	TYPE_DECL_NO_RBRACKET  ,// No closing array bracket
	TYPE_DECL_NO_ARRAY_SIZE,// Cannot find array size
	TYPE_DECL_NO_TYPE      ,// Cannot find type declaration

	FUNC_DECL_OPT_VOID    ,// Use of `void` is optional
	FUNC_DECL_NO_FUNC_NAME,// Cannot find fn name
	FUNC_DECL_NO_FUNCTION ,// No beginning 'function'
	FUNC_DECL_NO_SEMICOLON,// No ending semicolon

	FORMAL_PARAM_NO_LPAREN  ,// No opening parenthesis
	FORMAL_PARAM_NO_VAR_NAME,// Cannnot find var name
	FORMAL_PARAM_NO_COMMA   ,// Cannot find expected `,`
	FORMAL_PARAM_NO_RPAREN  ,// No closing parenthesis	

	FUNC_BODY_NO_LBRACE,// No brace to open function
	FUNC_BODY_NO_RBRACE,// No brace to close function

	STAT_SEQ_NO_SEMICOLON,// No separating semicolon

	STMT_UNRECOGNIZED_TERMINAL,// Unk terminal token found

	ASSIGNMENT_NO_LET   ,// No 'let' to denote assignment
	ASSIGNMENT_NO_LARROW,// No '<-' to act as assignment op

	DESIGNATOR_NO_NAME_REFERENCE,// No var name was specified
	DESIGNATOR_NO_LBRACKET      ,// No opening array bracket	
	DESIGNATOR_NO_RBRACKET      ,// No closing array bracket

	EXPR_EXPECTED_PLUS ,// No plus character
	EXPR_EXPECTED_MINUS,// No minus character

	TERM_EXPECTED_ASTERISK,// No asterisk character
	TERM_EXPECTED_SLASH   ,// No slash character

  FACTOR_NO_NUMBER      ,// No number when it was expected
	FACTOR_NO_LPAREN      ,// No '(' to open expression	
	FACTOR_NO_RPAREN      ,// No ')' to close expression
	FACTOR_UNK_TOKEN_FOUND,// Discovered unexpected token	

	FUNC_CALL_NO_CALL       ,// No beginning 'call'
	FUNC_CALL_NAME_REFERENCE,// No fn name was specified
	FUNC_CALL_NO_LPAREN     ,// No opening '(' for fn params
	FUNC_CALL_NO_COMMA      ,// No  `,` found when expected	
	FUNC_CALL_NO_RPAREN     ,// No closing ')' for fn params

	IF_STATEMENT_NO_IF  ,// No beginning 'if'
	IF_STATEMENT_NO_THEN,// No expected 'then'
	IF_STATEMENT_NO_ELSE,// No expected `else`
	IF_STATEMENT_NO_FI  ,// No ending 'fi'

	RELATION_NO_RELOP,// Relation operator missing

	WHILE_STATEMENT_NO_WHILE,// No beginning 'while'
	WHILE_STATEMENT_NO_DO   ,// No expected 'do'
	WHILE_STATEMENT_NO_OD   ,// No expected 'od'

	RETURN_STATEMENT_NO_RETURN,// No beginning 'return'

	UNK// Unknown parser error code
} ParserError;

typedef struct Parser {
	Lexer *lxr;
	TokenNode *curr_tkn; // pointer to most recently lexed token
	//FILE *fout; // outputfile QUESTION: IS THIS NEED INSIDE???
} Parser;

Parser *
new_parser (char *filename);

void
free_parser (Parser **p);

Ast *
parse (Parser *p);

bool
peek_tkn (TokenType t,
					Parser *p);

bool
peek_stmt_terminals (Parser *p);

void
consume_tkn (TokenType t,
						 Parser *p,
						 ParserError e);

Computation *
smpl_computation (Parser *p);

struct VarDecl *
smpl_var_decl (Parser *p);

NumList *
smpl_type_decl (Parser *p);

Num *
smpl_number (Parser *p,
						 ParserError e);

Ident *
smpl_ident (Parser *p,
						ParserError e);

struct FuncDecl *
smpl_func_decl (Parser *p);

IdentList *
smpl_formal_param (Parser *p);

FuncBody *
smpl_func_body (Parser *p);

StmtList *
smpl_stat_sequence (Parser *p);

Stmt *
smpl_statement (Parser *p);

Assignment *
smpl_assignment (Parser *p);

Designator *
smpl_designator (Parser *p);

struct Result *
smpl_expression (Parser *p);

struct Result *
smpl_term (Parser *p);

struct Result *
smpl_factor (Parser *p);

FuncCall *
smpl_func_call (Parser *p);

BinOp *
smpl_relation (Parser *p);

IfStmt *
smpl_if_statement (Parser *p);

WhileStmt *
smpl_while_statement (Parser *p);

ReturnStmt *
smpl_return_statement (Parser *p);

/* int */
/* val (char *c); */

void
throw_parser_error (Lexer *lxr,
										ParserError e);

/* int */
/* lookup (VAR vt[MAX_NUM_VARS], */
/* 				char *name); */

#endif // _PARSER_H_
