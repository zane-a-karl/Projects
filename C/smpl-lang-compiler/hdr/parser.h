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

#include "../hdr/var_table.h"
#include "../hdr/var_decls_list.h"
#include "../hdr/int_list.h"
#include "../hdr/str_list.h"

#include "../hdr/token.h"
#include "../hdr/ast.h"
#include "../hdr/basic_block.h"
#include "../hdr/dlx.h"
#include "../hdr/result.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* static const int MAX_NUM_VARS = 1<<8; //256 */
/* static const int MAX_VAR_NAME_LEN = 1<<4; //16 */
/* static const int MAX_LINE_LEN = 1<<8;//256 */
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

typedef struct Parser {
	FILE *fin;    // inputfile
	FILE *fout;   // outputfile
	TokenList *tl;
} Parser;

typedef enum ParserError {
	COMPUTATION_NO_LBRACE,// Cannot open main
	COMPUTATION_NO_RBRACE,// Cannot close main
	COMPUTATION_NO_MAIN  ,// No beginning 'main'
	COMPUTATION_NO_PERIOD,// No ending period

	VAR_DECL_NO_VAR_NAME ,// Cannnot find var name
	VAR_DECL_NO_SEMICOLON,// No ending semicolon

	TYPE_DECL_NO_LBRACKET  ,// No opening array bracket
	TYPE_DECL_NO_RBRACKET  ,// No closing array bracket
	TYPE_DECL_NO_ARRAY_SIZE,// Cannot find array size
	TYPE_DECL_NO_TYPE      ,// Cannot find type declaration

	FUNC_DECL_NO_FUNC_NAME,// Cannot find fn name
	FUNC_DECL_NO_FUNCTION ,// No beginning 'function'
	FUNC_DECL_NO_SEMICOLON,// No ending semicolon

	FORMAL_PARAM_NO_LPAREN,  // No opening parenthesis
	FORMAL_PARAM_NO_RPAREN,  // No closing parenthesis
	FORMAL_PARAM_NO_VAR_NAME,// Cannnot find var name

	FUNC_BODY_NO_LBRACE,// No brace to open function
	FUNC_BODY_NO_RBRACE,// No brace to close function

	ASSIGNMENT_NO_LET,   // No 'let' to denote assignment
	ASSIGNMENT_NO_LARROW,// No '<-' to act as assignment op

	DESIGNATOR_NO_NAME_REFERENCE,// No var name was specified
	DESIGNATOR_NO_RBRACKET      ,// No closing array bracket

	TERM_NO_RPAREN,// No closing parenthesis

	FUNC_CALL_NO_CALL       ,// No beginning 'call'
	FUNC_CALL_NAME_REFERENCE,// No fn name was specified
	FUNC_CALL_NO_RPAREN     ,// No closing ')' for fn params

	IF_STATEMENT_NO_IF  ,// No beginning 'if'
	IF_STATEMENT_NO_THEN,// No expected 'then'
	IF_STATEMENT_NO_FI  ,// No ending 'fi'

	RELATION_NO_RELOP,// Relation operator missing

	WHILE_STATEMENT_NO_WHILE,// No beginning 'while'
	WHILE_STATEMENT_NO_DO   ,// No expected 'do'
	WHILE_STATEMENT_NO_OD   ,// No expected 'od'

	RETURN_STATEMENT_NO_RETURN,// No beginning 'return'

	UNK// Unknown parser error code
} ParserError;

Parser *
init_parser (char *filename);// calloc

void
free_parser (Parser **p);

Ast *
parse (Parser *p);

void
smpl_computation (TokenNode **tn,
									Ast **ast);

void
smpl_var_decl (TokenNode **tn,
							 StrList **vl);

void
smpl_type_decl (TokenNode **tn,
								IntList **dl);

void
smpl_func_decl (TokenNode **tn,
								StrList **fl);

void
smpl_formal_param (TokenNode **tn);

void
smpl_func_body (TokenNode **tn);

void
smpl_stat_sequence (TokenNode **tn,
										Ast **);

void
smpl_statement (TokenNode **tn,
								Ast **subtree);

void
smpl_assignment (TokenNode **tn,
								 Ast **);

void
smpl_designator (TokenNode **tn,
								 BasicBlock **bb);

void
smpl_expression (TokenNode **tn,
								 BasicBlock **bb);

void
smpl_term (TokenNode **tn,
					 BasicBlock **bb);

void
smpl_factor (TokenNode **tn,
						 BasicBlock **bb);

void
smpl_func_call (TokenNode **tn,
								Ast **);

void
smpl_if_statement (TokenNode **tn,
									 Ast **);

void
smpl_while_statement (TokenNode **tn,
											Ast **);

void
smpl_relation (TokenNode **tn);

void
smpl_return_statement (TokenNode **tn,
											 Ast **);

/* int */
/* val (char *c); */

void
psr_err (int line,
				 ParserError e);

/* int */
/* lookup (VAR vt[MAX_NUM_VARS], */
/* 				char *name); */

bool
optional_token_type_is (TokenType type,
												TokenNode *tn);

void
mandatory_token_type_is (TokenType type,
												 TokenNode *tn,
												 ParserError e);

#endif // _PARSER_H_
