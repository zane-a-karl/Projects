/**
 * The EBNF for the respective morpheme is located above each 
 * function sharing the same name as the morpheme. I include the
 * "alphabet"-like terms here for convenience
 *  as they have already been taken into account during lexing.
 * letter = “a”|“b”|...|“z”.
 * digit = “0”|“1”|...|“9”.
 * relOp = “==“|“!=“|“<“|“<=“|“>“|“>=“.
 * ident = letter {letter | digit}.
 * number = digit {digit}.
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include "../hdr/token.h"
#include "../hdr/ast.h"
#include "../hdr/basic_block.h"
#include "../hdr/dlx.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* static const int MAX_NUM_VARS = 1<<8; //256 */
/* static const int MAX_NAME_LEN = 1<<4; //16 */
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

//Token \"<x>\" not found.
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

Ast *
parse (Parser *p);

void
smpl_computation (TokenNode *tn);

TokenNode *
smpl_var_decl (TokenNode *tn);

TokenNode *
smpl_type_decl (TokenNode *tn);

TokenNode *
smpl_func_decl (TokenNode *tn);

TokenNode *
smpl_formal_param (TokenNode *tn);

TokenNode *
smpl_func_body (TokenNode *tn);

TokenNode *
smpl_stat_sequence (TokenNode *tn);

TokenNode *
smpl_statement (TokenNode *tn);

TokenNode *
smpl_assignment (TokenNode *tn);

TokenNode *
smpl_designator (TokenNode *tn);

TokenNode *
smpl_expression (TokenNode *tn);

TokenNode *
smpl_term (TokenNode *tn);

TokenNode *
smpl_factor (TokenNode *tn);

TokenNode *
smpl_func_call (TokenNode *tn);

TokenNode *
smpl_if_statement (TokenNode *tn);

TokenNode *
smpl_while_statement (TokenNode *tn);

TokenNode *
smpl_relation (TokenNode *tn);

TokenNode *
smpl_return_statement (TokenNode *tn);

/* int */
/* val (char *c); */

void
psr_err (int line,
				 ParserError e);

/* int */
/* lookup (VAR vt[MAX_NUM_VARS], */
/* 				char *name); */

#endif // _PARSER_H_
