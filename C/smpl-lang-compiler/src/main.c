#include "../hdr/utils.h"
//#include "../hdr/var_table.h"
#include "../hdr/token.h"
#include "../hdr/lexer.h"
#include "../hdr/parser.h"
#include "../hdr/ast.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Global symbol lookup table.
//VarTable *vt;
// Global var for basic block number
int block_number;
// Global var for ssa instruction number
int ssa_number;

int
main (int argc,
			char *argv[])
{
	// Allocate memory for the first listing in the var table
	//vt = alloc_var_table();
	assert(argc == 2);

	// Allocates heap memory for parser
	Parser *psr = new_parser(argv[1]);

	// Allocates heap memory for ast
	Ast *ast = parse(psr);

	// Free heap memory
	free_parser(&psr);
	//	free_ast(&ast);

	return 0;
}
