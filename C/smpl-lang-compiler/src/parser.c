#include "../hdr/parser.h"

// Global symbol lookup table.
//extern VarTable *vt;

// Tkns that signal a stmt beginning
const TokenType stmt_terminals[] = {LET,
																		CALL,
																		IF,
																		WHILE,
																		RETURN};
const int num_stmt_terminals =
	sizeof(stmt_terminals)/sizeof(TokenType);

// Look up table for syntax error strings.
const char *parser_error_table[] = {
	"COMPUTATION_NO_LBRACE: Cannot open main",
	"COMPUTATION_NO_RBRACE: Cannot close main",
	"COMPUTATION_NO_MAIN  : No beginning 'main'",
	"COMPUTATION_NO_PERIOD: No ending period",

	"VAR_DECL_NO_VAR_NAME : Cannnot find var name",
	"VAR_DECL_NO_COMMA    : No separating comma",
	"VAR_DECL_NO_SEMICOLON: No ending semicolon",

	"TYPE_DECL_NO_VAR       : No prefixed var keyword",
	"TYPE_DECL_NO_ARRAY     : No prefixed array keyword",
	"TYPE_DECL_NO_LBRACKET  : No opening array bracket",
	"TYPE_DECL_NO_RBRACKET  : No closing array bracket",
	"TYPE_DECL_NO_ARRAY_SIZE: Cannot find array size",
	"TYPE_DECL_NO_TYPE      : Cannot find type declaration",

	"FUNC_DECL_OPT_VOID    : Use of `void` is optional",
	"FUNC_DECL_NO_FUNC_NAME: Cannot find fn name",
	"FUNC_DECL_NO_FUNCTION : No beginning 'function'",
	"FUNC_DECL_NO_SEMICOLON: No ending semicolon",

	"FORMAL_PARAM_NO_LPAREN  : No opening parenthesis",
	"FORMAL_PARAM_NO_COMMA   : Cannot find expected ','",
	"FORMAL_PARAM_NO_VAR_NAME: Cannnot find var name",
	"FORMAL_PARAM_NO_RPAREN  : No closing parenthesis",

	"FUNC_BODY_NO_LBRACE: No brace to open function",
	"FUNC_BODY_NO_RBRACE: No brace to close function",

	"STAT_SEQ_NO_SEMICOLON: No separating semicolon",

	"STMT_UNRECOGNIZED_TERMINAL: Unk terminal token found",

	"ASSIGNMENT_NO_LET:    No 'let' to denote assignment",
	"ASSIGNMENT_NO_LARROW: No '<-' to act as assignment op",

	"DESIGNATOR_NO_NAME_REFERENCE: No var name was specified",
	"DESIGNATOR_NO_LBRACKET      : No opening array bracket",
	"DESIGNATOR_NO_RBRACKET      : No closing array bracket",

	"EXPR_EXPECTED_PLUS : No plus character",
	"EXPR_EXPECTED_MINUS: No minus character",

	"TERM_EXPECTED_ASTERISK: No asterisk character",
	"TERM_EXPECTED_SLASH   : No slash character",

	"FACTOR_NO_NUMBER      : No number when it was expected",
	"FACTOR_NO_LPAREN      : No '(' to open expression",
	"FACTOR_NO_RPAREN      : No ')' to close expression",
	"FACTOR_UNK_TOKEN_FOUND: Discovered unexpected token",

	"FUNC_CALL_NO_CALL       : No beginning 'call'",
	"FUNC_CALL_NAME_REFERENCE: No fn name was specified",
	"FUNC_CALL_NO_LPAREN     : No closing '(' for fn params",
	"FUNC_CALL_NO_COMMA      : No ',' found when expected",
	"FUNC_CALL_NO_RPAREN     : No closing ')' for fn params",

	"IF_STATEMENT_NO_IF  : No beginning 'if'",
	"IF_STATEMENT_NO_THEN: No expected 'then'",
	"IF_STATEMENT_NO_ELSE: No expected 'else'",
	"IF_STATEMENT_NO_FI  : No ending 'fi'",

	"RELATION_NO_RELOP: Relation operator missing",

	"WHILE_STATEMENT_NO_WHILE: No beginning 'while'",
	"WHILE_STATEMENT_NO_DO   : No expected 'do'",
	"WHILE_STATEMENT_NO_OD   : No expected 'od'",

	"	RETURN_STATEMENT_NO_RETURN: No beginning 'return'",

	"UNK: Unknown parser error code"
};

Parser *
new_parser (char *filename)
{
	Parser *p   = calloc(1, sizeof(Parser));
	// Allocates heap memory to lexer
	p->lxr      = new_lexer(filename);
	p->curr_tkn = lex_next_tkn(p->lxr);
	/* p->fout = fopen("", "r"); */
	/* check_fopen(p->fout); */
	return p;
}

void
free_parser (Parser **p)
{
	free_lexer(&((*p)->lxr));
	// p->curr_tkn is free'd with the lexer
	/* fclose((*p)->fout); */
	free(*p);
}

Ast *
parse (Parser *p)
{
	// ONLY ast and ast->root calloc'd here
	Ast *ast = new_ast();
	ast->root->computation = smpl_computation(p);
	printf("Parsing Complete!\n");

	return ast;
}

bool
peek_tkn (TokenType t,
					Parser *p)
{
	return p->curr_tkn->tkn->type == t;
}

bool
peek_stmt_terminals (Parser *p)
{
	for (int i = 0; i < num_stmt_terminals; ++i) {
		if (p->curr_tkn->tkn->type == stmt_terminals[i])
			return true;
	}
	return false;
}

void
consume_tkn (TokenType t,
						 Parser *p,
						 ParserError e)
{
	if ( peek_tkn(t, p) ) {
		p->curr_tkn = lex_next_tkn(p->lxr);
	} else {
		throw_parser_error(p->lxr, e);
	}
}

// computation = "main" { varDecl } { funcDecl }
// "{" statSequence "}" "."
Computation *
smpl_computation (Parser *p)
{
	// ONLY c, vdl, and fdl calloc'd here
	Computation *c = new_computation();

	consume_tkn(MAIN, p, COMPUTATION_NO_MAIN);
	while ( peek_tkn(VAR, p) ||	peek_tkn(ARRAY, p) ) {
		push_vd(c->var_decls, smpl_var_decl(p));
	}
	while ( peek_tkn(FUNCTION, p) || peek_tkn(VOID, p) ) {
		push_fd(c->func_decls, smpl_func_decl(p));
	}
	consume_tkn(LBRACE, p, COMPUTATION_NO_LBRACE);
	c->stmts = smpl_stat_sequence(p);
	consume_tkn(RBRACE, p, COMPUTATION_NO_RBRACE);
	consume_tkn(PERIOD, p, COMPUTATION_NO_PERIOD);

	return c;
}

// varDecl = typeDecl ident { "," ident } ";"
struct VarDecl *
smpl_var_decl (Parser *p)
{
	// ONLY vd and il calloc'd here
	struct VarDecl *vd = new_vd();
	vd->dimensions = smpl_type_decl(p);
	push_ident(vd->identifiers,
						 smpl_ident(p, VAR_DECL_NO_VAR_NAME));

	while ( peek_tkn(COMMA, p) ) {
		consume_tkn(COMMA, p, VAR_DECL_NO_COMMA);
		push_ident(vd->identifiers,
							 smpl_ident(p, VAR_DECL_NO_VAR_NAME));
	}
	consume_tkn(SEMICOLON, p, VAR_DECL_NO_SEMICOLON);

	return vd;
}

// typeDecl = "var" |
// "array" "[" number "]" { "[" number "]"}
// Note: we only have to handle integer types and their
//  arrays because of this we can have the parser return
//  the list of dimensions of the identifier. For scalars
//  we would just  have an empty list, i.e.
//  list->head = NULL.
NumList *
smpl_type_decl (Parser *p)
{
	// ONLY nl calloc'd here
	NumList *dims = new_num_list();
	if ( peek_tkn(VAR, p) ) {
		consume_tkn(VAR, p, TYPE_DECL_NO_VAR);
		dims->head = NULL; // i.e. a scalar

	} else if ( peek_tkn(ARRAY, p) ) {
		consume_tkn(ARRAY, p, TYPE_DECL_NO_ARRAY);
		do {
			consume_tkn(LBRACKET, p, TYPE_DECL_NO_LBRACKET);
			push_num(dims,
							 smpl_number(p, TYPE_DECL_NO_ARRAY_SIZE));
			consume_tkn(RBRACKET, p, TYPE_DECL_NO_RBRACKET);
		} while ( peek_tkn(LBRACKET, p) );

	} else {
		throw_parser_error(p->lxr, TYPE_DECL_NO_TYPE);
	}

	return dims;
}

Num *
smpl_number (Parser *p,
						 ParserError e)
{
	// ONLY num calloc'd here
	Num *num = new_num();
	consume_tkn(NUMBER, p, e);
	num->val = p->curr_tkn->tkn->val;
	return num;
}

Ident *
smpl_ident (Parser *p,
						ParserError e)
{
	// ONLY ident and name calloc'd here
	Ident *ident = new_ident();
	consume_tkn(IDENT, p, e);
	for (int i = 0; i < MAX_VAR_NAME_LEN; ++i) {
		ident->name[i] = p->curr_tkn->tkn->raw[i];
	}
	return ident;
}

// funcDecl =
// [ "void" ] "function"
// ident formalParam ";" funcBody ";"
struct FuncDecl *
smpl_func_decl (Parser *p)
{
	// ONLY fd calloc'd here
	struct FuncDecl *fd = new_fd();
	if ( peek_tkn(VOID, p) ) {
		consume_tkn(VOID, p, FUNC_DECL_OPT_VOID);
		fd->is_void = true;
	}
	consume_tkn(FUNCTION, p, FUNC_DECL_NO_FUNCTION);
	fd->name    = smpl_ident(p, FUNC_DECL_NO_FUNC_NAME);
	fd->params  = smpl_formal_param(p);
	consume_tkn(SEMICOLON, p, FUNC_DECL_NO_SEMICOLON);
	fd->body    = smpl_func_body(p);
	consume_tkn(SEMICOLON, p, FUNC_DECL_NO_SEMICOLON);

	return fd;
}

// formalParam = "(" [ident { "," ident }] ")"
IdentList *
smpl_formal_param (Parser *p)
{
	// ONLY il calloc'd here
	IdentList *params = new_ident_list();
	consume_tkn(LPAREN, p, FORMAL_PARAM_NO_LPAREN);
	if ( peek_tkn(IDENT, p) ) {
		push_ident(params,
							 smpl_ident(p, FORMAL_PARAM_NO_VAR_NAME));
		while ( peek_tkn(COMMA, p) ) {
			consume_tkn(COMMA, p, FORMAL_PARAM_NO_COMMA);
			push_ident(params,
								 smpl_ident(p, FORMAL_PARAM_NO_VAR_NAME));
		} // endwhile
	} // endif
	consume_tkn(RPAREN, p, FORMAL_PARAM_NO_RPAREN);

	return params;
}

// funcBody = { varDecl } "{" [ statSequence ] "}"
FuncBody *
smpl_func_body (Parser *p)
{
	// ONLY fb and vdl calloc'd here
	FuncBody *fb = new_func_body();
	while ( peek_tkn(VAR, p) ||
					peek_tkn(ARRAY, p) ) {

		// new vdln calloc'd here
		push_vd(fb->local_vars, smpl_var_decl(p));
	}
	consume_tkn(LBRACE, p, FUNC_BODY_NO_LBRACE);
	fb->stmts = smpl_stat_sequence(p);
	consume_tkn(RBRACE, p, FUNC_BODY_NO_RBRACE);

	return fb;
}

// statSequence = statement { ";" statement } [ ";" ]
StmtList *
smpl_stat_sequence (Parser *p)
{
	// ONLY sl calloc'd here
	StmtList *sl = new_sl();
	push_stmt(sl, smpl_statement(p));
	while ( peek_tkn(SEMICOLON, p) ) {

		consume_tkn(SEMICOLON, p, STAT_SEQ_NO_SEMICOLON);
		if ( peek_stmt_terminals(p) ) {
			push_stmt(sl, smpl_statement(p));
		}
	}
	return sl;
}

// statement =
// assignment | "void" funcCall | ifStatement |
// whileStatement | returnStatement
Stmt *
smpl_statement (Parser *p)
{
	// ONLY Stmt calloc'd here
	Stmt *s = new_stmt();
	if ( peek_tkn(LET, p) ) {

		s->assignment = smpl_assignment(p);
	} else if ( peek_tkn(CALL, p) ) {

		s->func_call = smpl_func_call(p);
	} else if ( peek_tkn(IF, p) ) {

		s->if_stmt = smpl_if_statement(p);
	} else if ( peek_tkn(WHILE, p) ) {

		s->while_stmt = smpl_while_statement(p);
	} else if ( peek_tkn(RETURN, p) ) {

		s->return_stmt = smpl_return_statement(p);
	} else {
		throw_parser_error(p->lxr, STMT_UNRECOGNIZED_TERMINAL);
	}
	return s;
}

// assignment = "let" designator "<-" expression
Assignment *
smpl_assignment (Parser *p)
{
	// ONLY assignment calloc'd here
	Assignment *a = new_assignment();
	consume_tkn(LET, p,	ASSIGNMENT_NO_LET);
	a->lhs = smpl_designator(p);
	consume_tkn(LARROW, p, ASSIGNMENT_NO_LARROW);
	a->rhs = smpl_expression(p);
	return a;
}

// designator = ident{ "[" expression "]" }
Designator *
smpl_designator (Parser *p)
{
	// ONLY designator and result list calloc'd here
	Designator *d = new_designator();
	d->ident = smpl_ident(p, DESIGNATOR_NO_NAME_REFERENCE);
	while ( peek_tkn(LBRACKET, p) ) {

		consume_tkn(LBRACKET, p, DESIGNATOR_NO_LBRACKET);
		push_result(d->indices, smpl_expression(p));
		consume_tkn(RBRACKET, p, DESIGNATOR_NO_RBRACKET);
	}
	/////////////////////////////////////////////////////////
	// CHECK: CHECK for d->indices==NULL ?? Y/N??     ///// /
	/////////////////////////////////////////////////////////
	return d;
}

// expression = term {("+" | "-") term}
struct Result *
smpl_expression (Parser *p)
{
	// ONLY the `result` is calloc'd here
	struct Result *r = new_res();
	r->bin_op = new_bin_op();
	r->bin_op->lhs = smpl_term(p);
	while ( peek_tkn(PLUS, p) ||
					peek_tkn(MINUS, p) ) {

		r->bin_op->op = peek_tkn(PLUS, p) ? "+" : "-";
		if ( peek_tkn(PLUS, p) ) {
			consume_tkn(PLUS, p, EXPR_EXPECTED_PLUS);
		} else {
			consume_tkn(MINUS, p, EXPR_EXPECTED_MINUS);
		}
		r->bin_op->rhs = smpl_term(p);
	}
	return r;
}

// term = factor {("*" | "/") factor}
struct Result *
smpl_term (Parser *p)
{
	// ONLY the `result` is calloc'd here
	struct Result *r = new_res();
	r->bin_op = new_bin_op();
	r->bin_op->lhs = smpl_factor(p);
	while ( peek_tkn(ASTERISK, p) ||
					peek_tkn(SLASH, p) ) {

		r->bin_op->op =
			peek_tkn(ASTERISK, p) ? "*" : "/";
		if ( peek_tkn(ASTERISK, p) ) {
			consume_tkn(ASTERISK, p, TERM_EXPECTED_ASTERISK);
		} else {
			consume_tkn(SLASH, p, TERM_EXPECTED_SLASH);
		}
		r->bin_op->rhs = smpl_factor(p);
	}
	return r;
}

// factor = designator | number | "(" expression ")" |
// funcCall
// NOTE: only "non-void" fns may be called here.
struct Result *
smpl_factor (Parser *p)
{
	// ONLY the `result`, its `binop`, and the binop's `op`
	// are calloc'd here
	struct Result *r = new_res();
	if ( peek_tkn(IDENT, p) ) {

		r->des = smpl_designator(p);
	} else if ( peek_tkn(NUMBER, p) ) {

		r->num = smpl_number(p, FACTOR_NO_NUMBER);
	} else if ( peek_tkn(LPAREN, p) ) {

		consume_tkn(LPAREN, p, FACTOR_NO_LPAREN);
		r = smpl_expression(p);
		consume_tkn(RPAREN, p, FACTOR_NO_RPAREN);
	} else if ( peek_tkn(CALL, p) ) {

		r->func_call = smpl_func_call(p);
	} else {
		throw_parser_error(p->lxr, FACTOR_UNK_TOKEN_FOUND);
	}

	return r;
}

// funcCall =
// "call" ident [ "(" [expression
// { "," expression } ] ")" ]
// fns without params don't need "()" but can have them!
FuncCall *
smpl_func_call (Parser *p)
{
	// ONLY fc and fc->args calloc'd here
	FuncCall *fc = new_func_call();
	consume_tkn(CALL, p, FUNC_CALL_NO_CALL);
	fc->name = smpl_ident(p, FUNC_CALL_NAME_REFERENCE);
	if ( peek_tkn(LPAREN, p) ) {

		consume_tkn(LPAREN, p, FUNC_CALL_NO_LPAREN);
		push_result(fc->args, smpl_expression(p));
		while ( peek_tkn(COMMA, p) ) {

			consume_tkn(COMMA, p, FUNC_CALL_NO_COMMA);
			push_result(fc->args, smpl_expression(p));
		}
		consume_tkn(RPAREN, p, FUNC_CALL_NO_RPAREN);
	}
	return fc;
}

// relation = expression relOp expression
BinOp *
smpl_relation (Parser *p)
{
	// ONLY bo and bo->op calloc'd here
	BinOp *bo = new_bin_op();
	bo->lhs = smpl_expression(p);
	if ( peek_tkn(OP_INEQ, p) ||
			 peek_tkn(OP_EQ, p) ||
			 peek_tkn(OP_LT, p) ||
			 peek_tkn(OP_LE, p) ||
			 peek_tkn(OP_GT, p) ||
			 peek_tkn(OP_GE, p) ) {

		consume_tkn(p->curr_tkn->tkn->type, p, RELATION_NO_RELOP);
		// the op can only be two chars
		strncpy(bo->op, p->curr_tkn->tkn->raw, 2);
	}
	bo->rhs = smpl_expression(p);

	return bo;
}

// ifStatement =
// "if" relation "then"
// statSequence [ "else" statSequence ] "fi"
IfStmt *
smpl_if_statement (Parser *p)
{
	// ONLY ifstmt `is` calloc'd here
	IfStmt *is     = new_if_stmt();
	consume_tkn(IF, p, IF_STATEMENT_NO_IF);
	is->condition  = smpl_relation(p);
	consume_tkn(THEN, p, IF_STATEMENT_NO_THEN);
	is->then_stmts = smpl_stat_sequence(p);
	if ( peek_tkn(ELSE, p) ) {

		consume_tkn(ELSE, p, IF_STATEMENT_NO_ELSE);
		is->else_stmts = smpl_stat_sequence(p);
	}
	consume_tkn(FI, p, IF_STATEMENT_NO_FI);

	return is;
}

// whileStatement = "while" relation "do"
// StatSequence "od"
WhileStmt *
smpl_while_statement (Parser *p)
{
	// ONLY whilestmt `ws` calloc'd here
	WhileStmt *ws = new_while_stmt();
	consume_tkn(WHILE, p, WHILE_STATEMENT_NO_WHILE);
	ws->condition = smpl_relation(p);
	consume_tkn(DO, p, WHILE_STATEMENT_NO_DO);
	ws->do_stmts  = smpl_stat_sequence(p);
	consume_tkn(OD, p, WHILE_STATEMENT_NO_OD);

	return ws;
}

// returnStatement = "return" [ expression ]
ReturnStmt *
smpl_return_statement (Parser *p)
{
	// ONLY returnstmt `rs` calloc'd here
	ReturnStmt *rs = new_return_stmt();
	consume_tkn(RETURN, p, RETURN_STATEMENT_NO_RETURN);
	rs->ret_val = smpl_expression(p);

	return rs;
}

/* int */
/* val (char *c) { */

/* 	int rv; */
/* 	sscanf(c, "%d", &rv); */
/* 	return rv; */
/* } */

/* int */
/* lookup (VAR vt[MAX_NUM_VARS], */
/* 				char *name) { */
/* 	for (int i = 0; i < MAX_NUM_VARS; ++i) { */
/* 		if (strncmp(vt[i].name, name, MAX_NAME_LEN) == 0) { */
/* 			return vt[i].addr; */
/* 		} */
/* 	} */
/* 	return -1; */
/* } */

void
throw_parser_error (Lexer *lxr,
										ParserError e)
{
	printf("Error at (line=%i,col=%i)\n", lxr->line, lxr->col);
	perror(parser_error_table[e]);
	exit(1);
}
