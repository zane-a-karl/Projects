#include "../hdr/utils.h"
#include "../hdr/token.h"
#include "../hdr/lexer.h"
#include "../hdr/parser.h"
#include "../hdr/ast.h"

#include <stdio.h>
#include <stdlib.h>

int
main (int argc,
			char *argv[]) {

	Parser *p = (Parser *)calloc(1, sizeof(Parser));
	p->fin = fopen(argv[1], "r");
	check_fopen(p->fin);
	p->tl =	(TokenList *)calloc(1, sizeof(TokenList));
	p->tl = lex(p->fin);
	/* print_token_list(p->tl); */

	Ast *ast = (Ast *)calloc(1, sizeof(Ast));
	ast = parse(p);


	// Free heap memory
	free_token_list(p->tl);
	fclose(p->fin);
	/* fclose(p->fout); */

	return 0;
}
