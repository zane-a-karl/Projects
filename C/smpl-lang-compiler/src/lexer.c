#include "../hdr/lexer.h"

extern const char possible_individual_symbols[];
extern const char *possible_symbols[];

TokenList*
lex (FILE *fin) {

	TokenList *tl = (TokenList *)calloc(1, sizeof(TokenList));
	TokenNode *tn;
	Token *t;
	int ch;
	char buf[MAX_PROG_LINE_LEN];
	int cursor = 0;
	int line = 0;
	regex_t re_alnum;
	int rv_alnum = regcomp(&re_alnum,
												 "^[a-zA-Z0-9]*[:blank:]*$",
												 REG_EXTENDED);
	check_regex_compilation(rv_alnum);

  do {
    ch = fgetc(fin);
		check_ferror(fin);
		if ( triggered_feof(fin)  )     break;
		if ( found_whitespace(ch) )     continue;
		if ( found_newline(ch, &line) ) continue;
		if ( isalpha(ch) || isdigit(ch) ) {
			do {
				buf[cursor++] = ch;
				ch = fgetc(fin);
				check_ferror(fin);

			} while ( isalpha(ch) || isdigit(ch) );
			ungetc(ch, fin);           // No need for error check.
			if ( can_create_alnum_token(buf, &re_alnum) ) {
				t = init_token();        // alloc new mem for tkn
				create_alnum_token(t, buf, line);
				tn = init_token_node(t); // alloc new mem for tkn_node
				push_token_node(tl, tn);
			}
		} else if ( is_a_possible_individual_symbol(ch) ) {
			buf[cursor++] = ch;
			ch = fgetc(fin);
			check_ferror(fin);

			buf[cursor++] = ch; // could probably use a##b macro fn to do this and then you wouldn't need to clear erroneous char.
			if ( !can_create_symbol_token(buf) ) {
				ungetc(ch, fin);      // No need for error check.
				buf[--cursor] = '\0'; // clear erroneous character.
			}
			t = init_token();        // alloc new mem for tkn
			create_symbol_token(t, buf, line);
			tn = init_token_node(t); // alloc new mem for tkn_node
			push_token_node(tl, tn);

		} else {
			printf("Unknown character found: %c\n", ch);
		}

		memset(buf, '\0', strnlen(buf, MAX_PROG_LINE_LEN));
		cursor = 0;
  } while(1);

	printf("Completed tokenizing/lexing\n");
	return tl;
}

void
check_ferror (FILE *fin) {
	if (ferror(fin) != 0) {
		perror("Error reading from file");
		exit(1);
	}
}

bool
triggered_feof (FILE *fin) {
	if (feof(fin) != 0) {
		printf("Reached end of file\n");
		return true;
	}
	return false;
}

bool
found_whitespace (char ch) {
	if (ch == ' ' || ch == '\t') {
		return true;
	}
	return false;
}

bool
found_newline (char ch,
							 int *line) {
	if (ch == '\n') {
		++(*line);
		return true;
	}
	return false;
}

bool
can_create_alnum_token (char *buf,
												regex_t *re_alnum) {
	return regexec(re_alnum, buf, 0, NULL, 0) ? false : true;
}

bool
is_a_possible_individual_symbol (char c) {
	int num_possible_individual_symbols = 17;
	for (int i = 0; i < num_possible_individual_symbols; ++i) {
		if (c == possible_individual_symbols[i]) {
			return true;
		}
	}
	return false;
}

bool
can_create_symbol_token (char *buf) {
	int num_possible_symbols = 20;
	for (int i = 0; i < num_possible_symbols; ++i) {
		if (strncmp(buf, possible_symbols[i], 2) == 0) {
			return true;
		}
	}
	return false;
}
