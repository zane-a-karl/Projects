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

enum ParserSector {
	COMPUTATION,
	VAR_DECL,
	TYPE_DECL,
	FUNC_DECL,
	FORMAL_PARAM,
	FUNC_BODY,
	STAT_SEQ,
	STMT,
	ASSIGNMENT,
	DESIGNATOR,
	EXPRESSION,
	TERM,
	FACTOR,
	FUNC_CALL,
	IF_STMT,
	RELATION,
	WHILE_STMT,
	RETURN_STMT
};

struct Parser {
	struct Lexer *lxr;
	struct TokenNode *curr_tkn; // pointer to most recently lexed token
	//FILE *fout; // outputfile QUESTION: IS THIS NEED INSIDE???
};

struct Parser *
new_parser (char *filename);

void
free_parser (struct Parser **p);

bool
peek_tkn (enum TokenType t,
					struct Parser *p);

bool
peek_stmt_terminals (struct Parser *p);

void
consume_tkn (enum TokenType t,
						 struct Parser *p,
						 enum ParserSector ps);

struct Ast *
parse (struct Parser *p);

struct AstNode *
smpl_computation (struct Parser *p,
									struct Ast *ast);

struct VarDecl *
smpl_var_decl (struct Parser *p);

struct NumberList *
smpl_type_decl (struct Parser *p);

struct Number *
smpl_number (struct Parser *p,
						 enum ParserSector ps);

struct Ident *
smpl_ident (struct Parser *p,
						enum ParserSector ps);

struct FuncDecl *
smpl_func_decl (struct Parser *p);

struct IdentList *
smpl_formal_param (struct Parser *p);

struct FuncBody *
smpl_func_body (struct Parser *p);

struct StmtList *
smpl_stat_sequence (struct Parser *p);

struct Stmt *
smpl_statement (struct Parser *p);

struct Assignment *
smpl_assignment (struct Parser *p);

struct Designator *
smpl_designator (struct Parser *p);

struct Result *
smpl_expression (struct Parser *p);

struct Result *
smpl_term (struct Parser *p);

struct Result *
smpl_factor (struct Parser *p);

struct FuncCall *
smpl_func_call (struct Parser *p);

struct BinOp *
smpl_relation (struct Parser *p);

struct IfStmt *
smpl_if_statement (struct Parser *p);

struct WhileStmt *
smpl_while_statement (struct Parser *p);

struct ReturnStmt *
smpl_return_statement (struct Parser *p);

/* int */
/* val (char *c); */

void
throw_parser_error (enum TokenType t,
										struct Parser *p,
										enum ParserSector ps);

/* int */
/* lookup (VAR vt[MAX_NUM_VARS], */
/* 				char *name); */

#endif // _PARSER_H_
