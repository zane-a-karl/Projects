#include "../hdr/lexer.h"

extern const char possible_individual_symbols[];
extern const int num_possible_individual_symbols;
extern const char *possible_symbols[];
extern const int num_possible_symbols;

Lexer *
new_lexer (char *input_filename)
{
	Lexer *lxr   = calloc(1, sizeof(Lexer));
	lxr->fin     = fopen(input_filename, "r");
	// Allocates heap memory for a token list
	lxr->tl      = new_token_list();
	memset(lxr->buf, '\0', strnlen(lxr->buf, MAX_TKN_LEN));
	lxr->pos     = 0;
	lxr->tkn_num = 0;
	lxr->line    = 0;
	lxr->col     = -1;

	return lxr;
}

void
free_lexer (Lexer **lxr)
{
	fclose((*lxr)->fin);
	free_token_list((*lxr)->tl);
	free((*lxr));
}

/**
 *  Lexes a single token from the file attached to `lxr->fin`
 * and adds it to the end of the token list, `lxr->tl`.
 * Returns NULL if we've reached the end of the file.
 */
TokenNode *
lex_next_tkn (Lexer *lxr)
{
	Token *t;
	int ch;
	regex_t re_alnum;
	int rv_alnum = regcomp(&re_alnum,
												 "^[a-zA-Z0-9]*[:blank:]*$",
												 REG_EXTENDED);
	check_regex_compilation(rv_alnum);

	memset(lxr->buf, '\0', strnlen(lxr->buf, MAX_TKN_LEN));
	lxr->pos = 0;
	lxr->tkn_num++;

	// Pre-trim leading whitespace
	do {
		ch = fgetc(lxr->fin);
		check_ferror(lxr->fin);
		if ( triggered_feof(lxr->fin) ) return NULL;
		lxr->col++;
	} while ( found_whitespace(ch)
						|| found_newline(ch, lxr) );

	// Handle finding char or digit
	if ( isalpha(ch) || isdigit(ch) ) {

		do {
			lxr->buf[lxr->pos] = ch;
			lxr->pos++;
			ch = fgetc(lxr->fin);
			check_ferror(lxr->fin);
			lxr->col++;
		} while ( isalpha(ch) || isdigit(ch) );

		// Unget() non-alpha/digit char
		ungetc(ch, lxr->fin);
		lxr->col--;

		// Check alpha numeric token creation
		if ( can_create_alnum_token(lxr->buf, &re_alnum) ) {

			// Allocates heap mem for a token
			t = new_alnum_token(lxr->buf, lxr->line);
			return push_token(lxr->tl, t);

		} else {
			printf("Unknown tkn seq(%d, %d): \"%s\"\n",
						 lxr->line,
						 lxr->col,
						 lxr->buf);
			exit(1);
			return NULL;
		}

		// Handle finding a symbol
	} else if ( is_a_possible_individual_symbol(ch) ) {

		do {
			lxr->buf[lxr->pos] = ch;
			lxr->pos++;
			ch = fgetc(lxr->fin);
			check_ferror(lxr->fin);
			lxr->col++;
		} while ( is_a_possible_symbol(lxr->buf) );

		// Unget() non-indiv-symbol char
		ungetc(ch, lxr->fin);
		lxr->col--;

		// Check symbol token creation
		if ( can_create_symbol_token(lxr->buf) ) {

			// Allocates heap mem for a token
			t =  new_symbol_token(lxr->buf, lxr->line);
			return push_token(lxr->tl, t);

		} else {
			printf("Unknown tkn seq(%d, %d): \"%s\"\n",
						 lxr->line,
						 lxr->col,
						 lxr->buf);
			exit(1);
			return NULL;
		}

	} else {
		printf("Unknown char found(%d, %d): \"%c\"\n",
					 lxr->line,
					 lxr->col,
					 ch);
		exit(1);
		return NULL;
	}
}

void
check_ferror (FILE *fin)
{
	if (ferror(fin) != 0) {
		perror("Error reading from file");
		exit(1);
	}
}

bool
triggered_feof (FILE *fin)
{
	if (feof(fin) != 0) {
		printf("Reached end of file\n");
		return true;
	}
	return false;
}

bool
found_whitespace (char ch)
{
	if (ch == ' ' || ch == '\t') {
		return true;
	}
	return false;
}

bool
found_newline (char ch,
							 Lexer *l)
{
	if (ch == '\n' || ch == '\r') {
		l->line++;
		l->col = 0;
		return true;
	}
	return false;
}

bool
can_create_alnum_token (char *buf,
												regex_t *re_alnum)
{
	return regexec(re_alnum, buf, 0, NULL, 0) ? false : true;
}

bool
is_a_possible_individual_symbol (char c)
{
	for (int i = 0; i < num_possible_individual_symbols; ++i) {
		if (c == possible_individual_symbols[i])
			return true;
	}
	return false;
}

bool
is_a_possible_symbol (char *buf)
{
	for (int i = 0; i < num_possible_symbols; ++i) {
		if ( strncmp(buf, possible_symbols[i], 2) )
			return true;
	}
	return false;
}

bool
can_create_symbol_token (char *buf)
{
	int num_possible_symbols = 20;
	for (int i = 0; i < num_possible_symbols; ++i) {
		if (strncmp(buf, possible_symbols[i], 2) == 0) {
			return true;
		}
	}
	return false;
}
