#include "../hdr/token.h"


const char possible_individual_symbols[] = {
	'!', '=', '<', '>', '[', ']', '(', ')', '{',
	'}', '*', '/', '+', '-', ';', '.', ','
};
const char *possible_symbols[] = {
	"!=", "==", "<", "<=", ">",	">=",  "[", "]", "(", ")",
	"{" ,  "}", "*",  "/", "+",	 "-", "<-", ";", ".", ","
};

Token *
init_token () {

	Token *t = calloc(1, sizeof(Token));
	t->type = 0;
	t->raw = calloc(MAX_PROG_LINE_LEN, sizeof(char));
	t->val = 0;
	t->line = 0;
	return t;
}

TokenNode *
init_token_node (Token *t) {

	TokenNode *tn = calloc(1,	sizeof(TokenNode));
	tn->tkn = t;
	tn->next = NULL;
	return tn;
}

void
next_token (TokenNode **tn) {

	(*tn) = (*tn)->next;
}

void
clear_token_node (TokenNode *tn) {

	tn->tkn->type    = 0;
	tn->tkn->raw = calloc(MAX_PROG_LINE_LEN, sizeof(char));
	tn->tkn->val     = 0;
	tn->tkn->line    = 0;
	tn->next = NULL;
}

void
push_token_node (TokenList *tl,
								 TokenNode *new_node) {

	TokenNode *i = tl->head;
	if ( i == NULL ) {
		tl->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i=i->next;
		}
		i->next = new_node;
	}
}

void
print_token_list (TokenList *tl) {

	int idx = 0;
  for (TokenNode *i = tl->head; i != NULL; i=i->next) {
    printf("Tkn %d: type=%d, raw=\"%s\", val=%i, line=%i\n",
           idx++,
           i->tkn->type,
           i->tkn->raw,
           i->tkn->val,
           i->tkn->line);
  }
}

void
free_token_list (TokenList *tl) {

	TokenNode *cur = tl->head;
	TokenNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free(prv);
	}
}

void
create_alnum_token (Token *t,
										char *buf,
										int line) {

	regex_t re_ident, re_number;
	int rv_ident = regcomp(&re_ident,
												 "^[a-zA-Z][a-zA-Z0-9]*$",
												 REG_EXTENDED);
	check_regex_compilation(rv_ident);
	int rv_number = regcomp(&re_number,
												 "^[0-9]+$",
												 REG_EXTENDED);
	check_regex_compilation(rv_number);
	strncpy(t->raw, buf, MAX_PROG_LINE_LEN);

	t->line = line;

	if ( strncmp(buf, "main", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = MAIN;
	} else if ( strncmp(buf, "function", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = FUNCTION;
	} else if ( strncmp(buf, "void", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = VOID;
	} else if ( strncmp(buf, "array", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = ARRAY;
	} else if ( strncmp(buf, "var", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = VAR;
	} else if ( strncmp(buf, "return", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = RETURN;
	} else if ( strncmp(buf, "od", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = OD;
	} else if ( strncmp(buf, "do", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = DO;
	} else if ( strncmp(buf, "while", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = WHILE;
	} else if ( strncmp(buf, "fi", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = FI;
	} else if ( strncmp(buf, "else", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = ELSE;
	} else if ( strncmp(buf, "then", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = THEN;
	} else if ( strncmp(buf, "if", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = IF;
	} else if ( strncmp(buf, "call", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = CALL;
	} else if ( strncmp(buf, "let", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = LET;
	} else if ( !regexec(&re_number, buf, 0, NULL, 0) ) {
		t->type = NUMBER;
		sscanf(buf, "%i", &(t->val));
	} else if ( !regexec(&re_ident, buf, 0, NULL, 0) ) {
		t->type = IDENT;
	} else {
		perror("Unknown character sequence");
	}
}

void
create_symbol_token (Token *t,
										 char *buf,
										 int line) {

	strncpy(t->raw, buf, MAX_PROG_LINE_LEN);

	t->line = line;

	if ( strncmp(buf, "!=", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = OP_INEQ;
	} else if ( strncmp(buf, "==", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = OP_EQ;
	} else if ( strncmp(buf, "<", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = OP_LT;
	} else if ( strncmp(buf, "<=", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = OP_LE;
	} else if ( strncmp(buf, ">", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = OP_GT;
	} else if ( strncmp(buf, ">=", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = OP_GE;
	} else if ( strncmp(buf, "[", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = LBRACKET;
	} else if ( strncmp(buf, "]", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = RBRACKET;
	} else if ( strncmp(buf, "(", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = LPAREN;
	} else if ( strncmp(buf, ")", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = RPAREN;
	} else if ( strncmp(buf, "{", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = LBRACE;
	} else if ( strncmp(buf, "}", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = RBRACE;
	} else if ( strncmp(buf, "*", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = ASTERISK;
	} else if ( strncmp(buf, "/", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = SLASH;
	} else if ( strncmp(buf, "+", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = PLUS;
	} else if ( strncmp(buf, "-", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = MINUS;
	} else if ( strncmp(buf, "<-", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = LARROW;
	} else if ( strncmp(buf, ";", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = SEMICOLON;
	} else if ( strncmp(buf, ".", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = PERIOD;
	} else if ( strncmp(buf, ",", MAX_PROG_LINE_LEN) == 0 ) {
		t->type = COMMA;
	} else {
		perror("Unrecognized symbol\n");
	}
}

void
check_regex_compilation (int rv) {

	if (rv) {
		perror("Could not compile regex\n");
		exit(1);
	}
}
