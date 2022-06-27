#ifndef _LEXER_H_
#define _LEXER_H_

#include "../hdr/token.h"

#include <regex.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_PROG_LINE_LEN
#define MAX_PROG_LINE_LEN 256
#endif//MAX_PROG_LINE_LEN

TokenList *
lex (FILE *fin);

/* void */
/* tokenize (FILE *fin); */

void
check_ferror (FILE *fin);

bool
triggered_feof (FILE *fin);

bool
found_whitespace (char ch);

bool
found_newline (char ch,
							 int *line);

bool
can_create_alnum_token (char *buf,
												regex_t *re_alnum);

bool
is_a_possible_individual_symbol (char c);

bool
can_create_symbol_token (char *buf);

#endif//_LEXER_H_
