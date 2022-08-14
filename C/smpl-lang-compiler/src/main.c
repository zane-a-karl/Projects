#include "../hdr/utils.h"
#include "../hdr/token.h"
#include "../hdr/lexer.h"
#include "../hdr/parser.h"
#include "../hdr/dot_utils.h"

#include <assert.h>
#include <graphviz/cgraph.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc,
			char *argv[])
{
	assert(argc == 2);

	// Allocates heap memory for parser
	struct Parser *psr = new_parser(argv[1]);

	Agraph_t *tlg = agopen("Top Level Graph", Agdirected, NULL);
	// Allocates heap memory for ast
	struct Ast *ast = parse(psr, tlg);
	gen_dot_graph(ast, stdout);
	//	agwrite(tlg, stdout);

	// Free heap memory
	free_parser(&psr);
	free_ast(&ast);

	return 0;
}
