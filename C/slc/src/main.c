#include "../hdr/utils.h"
//#include "../hdr/var_table.h"
#include "../hdr/token.h"
#include "../hdr/lexer.h"
#include "../hdr/parser.h"
#include "../hdr/ast.h"

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
			char *argv[]) {

	// Allocate memory for the first listing in the var table
	//vt = alloc_var_table();

	Parser *p = init_parser(argv[1]);
	p->tl = lex(p->fin);
	/* print_token_list(p->tl); */

	Ast *ast = parse(p);// calloc'd here

	// Free heap memory
	free_parser(&p);

	return 0;
}
