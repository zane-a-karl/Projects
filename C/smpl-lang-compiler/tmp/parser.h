#ifndef _PARSER_H_
#define _PARSER_H_

//#include "../hdr/hash_table.h"
//#include "../hdr/stack.h"
//#include "../hdr/result.h"
//#include "../hdr/dlx_utils.h"
#include "../hdr/token.h"

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

typedef enum ParserError {
	NOCLOSINGPAREN,
	UNK
} ParserError;

/* typedef struct VAR { */
/* 	char *name; */
/* 	int addr; */
/* } VAR; */

typedef struct Parser {
	FILE *fin;
	FILE *fout;
	char line[MAX_LINE_LEN];
	int pos; // Position w/n line.
	//	Stack instrs;
	//Variable *var_table[MAX_NUM_VARS];
	//	VAR var_table[MAX_NUM_VARS];
	//	Register memory[NUM_REGISTERS];
} Parser;

void
parse (Parser *p);

Result
E (Parser *p);

Result
T (Parser *p);

Result
F (Parser *p);

void
next_char (Parser *p);

int
val (char *c);

void
error (ParserError e);

int
lookup (VAR vt[MAX_NUM_VARS],
				char *name);

#endif // _PARSER_H_
