#include "../hdr/utils.h"
#include "../hdr/token.h"
#include "../hdr/lexer.h"
//#include "../hdr/parser.h"

#include <stdio.h>

int
main (int argc,
			char *argv[]) {

	FILE *inputfile = fopen("input/test.smpl", "r");
	if ( inputfile == NULL ) printf("couldn't open the file\n");
	TokenList *tl =	(TokenList *)calloc(1, sizeof(TokenList));
	tl =	lex(inputfile);
	print_token_list(tl);

	/* Parser p; */
	/* p.fin = fopen("input/a_plus_b_times_3_plus_4.txt", "r"); */
	/* handle_return_FILE_error("error opening fin", p.fin); */
	/* p.fout = fopen("output/a_plus_b_times_3_plus_4.txt", "w"); */
	/* handle_return_FILE_error("error opening fout", p.fout); */

	// TODO: zero out p.line
	// TODO: zero out p.var_table
	// TODO: zero out p.memory and set it up

	/* p.var_table[0].name = "a"; */
	/* p.var_table[0].addr = 23; */
	/* p.var_table[1].name = "b"; */
	/* p.var_table[1].addr = 13; */

	/* parse(&p); */

	/* fclose(p.fin); */
	/* fclose(p.fout); */
	/* free(p.line); */

	return 0;
}
