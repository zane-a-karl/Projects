#include "../hdr/parser.h"

// Global symbol lookup table.
//extern VarTable *vt;

// Look up table for syntax error strings.
const char *parser_error_table[] = {
	"COMPUTATION_NO_LBRACE: Cannot open main",
	"COMPUTATION_NO_RBRACE: Cannot close main",
	"COMPUTATION_NO_MAIN  : No beginning 'main'",
	"COMPUTATION_NO_PERIOD: No ending period",

	"VAR_DECL_NO_VAR_NAME : Cannnot find var name",
	"VAR_DECL_NO_SEMICOLON: No ending semicolon",

	"TYPE_DECL_NO_LBRACKET  : No opening array bracket",
	"TYPE_DECL_NO_RBRACKET  : No closing array bracket",
	"TYPE_DECL_NO_ARRAY_SIZE: Cannot find array size",
	"TYPE_DECL_NO_TYPE      : Cannot find type declaration",

	"FUNC_DECL_NO_FUNC_NAME: Cannot find fn name",
	"FUNC_DECL_NO_FUNCTION : No beginning 'function'",
	"FUNC_DECL_NO_SEMICOLON: No ending semicolon",

	"FORMAL_PARAM_NO_LPAREN:   No opening parenthesis",
	"FORMAL_PARAM_NO_RPAREN:   No closing parenthesis",
	"FORMAL_PARAM_NO_VAR_NAME: Cannnot find var name",

	"FUNC_BODY_NO_LBRACE: No brace to open function",
	"FUNC_BODY_NO_RBRACE: No brace to close function",

	"ASSIGNMENT_NO_LET:    No 'let' to denote assignment",
	"ASSIGNMENT_NO_LARROW: No '<-' to act as assignment op",

	"DESIGNATOR_NO_NAME_REFERENCE: No var name was specified",
	"DESIGNATOR_NO_RBRACKET      : No closing array bracket",

	"TERM_NO_RPAREN:  No closing parenthesis",

	"FACTOR_UNK_TOKEN_FOUND: Discovered unexpected token",
	"FACTOR_NO_RPAREN      : No ')' to close expression",

	"FUNC_CALL_NO_CALL       : No beginning 'call'",
	"FUNC_CALL_NAME_REFERENCE: No fn name was specified",
	"FUNC_CALL_NO_RPAREN     : No closing ')' for fn params",

	"IF_STATEMENT_NO_IF  : No beginning 'if'",
	"IF_STATEMENT_NO_THEN: No expected 'then'",
	"IF_STATEMENT_NO_FI  : No ending 'fi'",

	"RELATION_NO_RELOP: Relation operator missing",

	"WHILE_STATEMENT_NO_WHILE: No beginning 'while'",
	"WHILE_STATEMENT_NO_DO   : No expected 'do'",
	"WHILE_STATEMENT_NO_OD   : No expected 'od'",

	"	RETURN_STATEMENT_NO_RETURN: No beginning 'return'",

	"UNK: Unknown parser error code"
};

Parser *
init_parser(char *filename) {

	Parser *p = calloc(1, sizeof(Parser));
	p->fin = fopen(filename, "r");
	check_fopen(p->fin);
	/* p->fout = fopen("", "r"); */
	/* check_fopen(p->fout); */
	p->tl =	calloc(1, sizeof(TokenList));
	return p;
}

void
free_parser (Parser **p) {

	free_token_list((*p)->tl);
	fclose((*p)->fin);
	/* fclose((*p)->fout); */
	free(*p);
}

Ast *
parse (Parser *p) {

	Ast *ast = init_ast();
	// Might need to deep copy p->tl->head into another
	// tokennode pointer in order to no "use up" the token

	ast->root->comp = smpl_computation(&(p->tl->head));
	printf("Parsing Complete!\n");
	return ast;
}

// computation = "main" { varDecl } { funcDecl }
// "{" statSequence "}" "."
Computation *
smpl_computation (TokenNode **tn) {

	Computation *c = init_computation();

	assert_token_type_is(MAIN, tn,
											 COMPUTATION_NO_MAIN);
	next_token(tn);
	while ( token_type_is(VAR, tn) ||
					token_type_is(ARRAY, tn) ) {

		push_vdl_data(&c->var_decls, smpl_var_decl(tn));
	}
	while ( token_type_is(FUNCTION, tn) ||
					token_type_is(VOID, tn) ) {

		push_fdl_data(&c->func_decls, smpl_func_decl(tn));
	}
	assert_token_type_is(LBRACE, tn,
											 COMPUTATION_NO_LBRACE);
	next_token(tn);
	c->stmts = smpl_stat_sequence(tn);
	assert_token_type_is(RBRACE, tn,
											 COMPUTATION_NO_RBRACE);
	next_token(tn);
	assert_token_type_is(PERIOD, tn,
											 COMPUTATION_NO_PERIOD);
	next_token(tn);
	return c;
}

// varDecl = typeDecl ident { "," ident } ";"
struct VarDecl *
smpl_var_decl (TokenNode **tn) {

	// New vd, num_list, ident_list calloc'd here
	struct VarDecl *vd = init_vd();
	// TODO: handle doulbe calloc dims here!
	vd->dimensions = smpl_type_decl(tn);
	assert_token_type_is(IDENT, tn,
											 VAR_DECL_NO_VAR_NAME);
	// New ident_node calloc'd here
	push_ident_list_data(&vd->identifiers,
											 build_ident((*tn)->tkn->raw));
	next_token(tn);
	while ( token_type_is(COMMA, tn) ) {
		next_token(tn);
		assert_token_type_is(IDENT, tn,
												 VAR_DECL_NO_VAR_NAME);
		// New ident_node calloc'd here
		push_ident_list_data(&vd->identifiers,
												 build_ident((*tn)->tkn->raw));
		next_token(tn);
	}
	assert_token_type_is(SEMICOLON, tn,
											 VAR_DECL_NO_SEMICOLON);
	next_token(tn);
	return vd;
}

// typeDecl = "var" |
// "array" "[" number "]" { "[" number "]"}
// Note: we only have to handle integer types and their
//  arrays because of this we can have the parser return
//  the list of dimensions of the identifier. For scalars
//  we would just  have an empty list, i.e.
//  list->head = NULL.
NumList */*for the dimensionts list*/
smpl_type_decl (TokenNode **tn) {

	NumList *dims = init_num_list();
	if ( token_type_is(VAR, tn) ) {

		dims->head = NULL; // i.e. a scalar
		next_token(tn);
	} else if ( token_type_is(ARRAY, tn) ) {

		next_token(tn);
		do {

			assert_token_type_is(LBRACKET, tn,
													 TYPE_DECL_NO_LBRACKET);
			next_token(tn);
			assert_token_type_is(NUMBER, tn,
													 TYPE_DECL_NO_ARRAY_SIZE);
			// New node calloc'd here
			push_num_list_data(&dims,
												 build_num((*tn)->tkn->val));
			next_token(tn);
			assert_token_type_is(RBRACKET, tn,
													 TYPE_DECL_NO_RBRACKET);
			next_token(tn);

		} while ( token_type_is(LBRACKET, tn) );

	} else { psr_err(tn, TYPE_DECL_NO_TYPE);}
	return dims;
}

// funcDecl =
// [ "void" ] "function"
// ident formalParam ";" funcBody ";"
struct FuncDecl *
smpl_func_decl (TokenNode **tn) {

	// New fd, ident, ident_list, vdl, sl calloc'd here
	struct FuncDecl *fd = init_fd();
	// No error because "void" is optional
	if ( token_type_is(VOID, tn) ) {

		fd->is_void = true;
		next_token(tn);
	}
	assert_token_type_is(FUNCTION, tn,
											 FUNC_DECL_NO_FUNCTION);
	next_token(tn);
	assert_token_type_is(IDENT, tn,
											 FUNC_DECL_NO_FUNC_NAME);
	// This sets the fn name TODO: handle double calloc
	fd->ident = build_ident((*tn)->tkn->raw);
	next_token(tn);
	// TODO: handle double calloc
	fd->params = smpl_formal_param(tn);
	assert_token_type_is(SEMICOLON, tn,
											 FUNC_DECL_NO_SEMICOLON);
	next_token(tn);
	// TODO: check if there's a double calloc here
	FuncBody *fb   = smpl_func_body(tn);
	fd->local_vars = fb->local_vars;
	fd->stmts      = fb->stmts;
	assert_token_type_is(SEMICOLON, tn,
											 FUNC_DECL_NO_SEMICOLON);
	next_token(tn);
	return fd;
}

// formalParam = "(" [ident { "," ident }] ")"
IdentList * /*for the param list*/
smpl_formal_param (TokenNode **tn) {

	IdentList *params = init_ident_list();
	assert_token_type_is(LPAREN, tn,
											 FORMAL_PARAM_NO_LPAREN);
	next_token(tn);
	if ( token_type_is(IDENT, tn) ) {

		// New ident_node calloc'd here
		push_ident_list_data(&params,
												 build_ident((*tn)->tkn->raw));
		next_token(tn);
		while ( token_type_is(COMMA, tn) ) {

			next_token(tn);
			assert_token_type_is(IDENT, tn,
													 FORMAL_PARAM_NO_VAR_NAME);

			// New ident_node calloc'd here
			push_ident_list_data(&params,
													 build_ident((*tn)->tkn->raw));
			next_token(tn);
		}
	} // No error b/c param's are optional
	assert_token_type_is(RPAREN, tn,
											 FORMAL_PARAM_NO_RPAREN);
	next_token(tn);
	return params;
}

// funcBody = { varDecl } "{" [ statSequence ] "}"
FuncBody *
smpl_func_body (TokenNode **tn) {

	// new fb, vdl, sl calloc'd here
	FuncBody *fb = init_func_body();
	while ( token_type_is(VAR, tn) ||
					token_type_is(ARRAY, tn) ) {

		// new vdln calloc'd here
		push_vdl_data(&fb->local_vars, smpl_var_decl(tn));
	}
	assert_token_type_is(LBRACE, tn,
											 FUNC_BODY_NO_LBRACE);
	next_token(tn);
	// Stmt can be empty
	// new sl calloc'd here TODO: check for double calloc
	fb->stmts = smpl_stat_sequence(tn);
	assert_token_type_is(RBRACE, tn,
											 FUNC_BODY_NO_RBRACE);
	next_token(tn);
	return fb;
}

// statSequence = statement { ";" statement } [ ";" ]
StmtList *
smpl_stat_sequence (TokenNode **tn) {

	StmtList *sl = init_sl();
	push_sl_data(&sl, smpl_statement(tn));
	while ( token_type_is(SEMICOLON, tn) ) {

		next_token(tn);
		push_sl_data(&sl, smpl_statement(tn));
	}
	// No error b/c terminating ";"s are optional
	if ( token_type_is(SEMICOLON, tn) ) {

		next_token(tn);
	}
	return sl;
}

// statement =
// assignment | "void" funcCall | ifStatement |
// whileStatement | returnStatement
Stmt *
smpl_statement (TokenNode **tn) {

	// new stmt and assoc'd fields calloc'd here
	Stmt *s = init_stmt();
	if ( token_type_is(LET, tn) ) {

		// new assignment stmt calloc'd here
		s->assignment = smpl_assignment(tn);
	} else if ( token_type_is(CALL, tn) ) {

		// new func call stmt calloc'd here
		s->func_call = smpl_func_call(tn);
	} else if ( token_type_is(IF, tn) ) {

		// new if stmt calloc'd here
		s->if_stmt = smpl_if_statement(tn);
		// new if stmt calloc'd here
	} else if ( token_type_is(WHILE, tn) ) {

		// new while stmt calloc'd here
		s->while_stmt = smpl_while_statement(tn);
	} else if ( token_type_is(RETURN, tn) ) {

		// new return stmt calloc'd here
		s->return_stmt = smpl_return_statement(tn);
	}
	return s;
}

// assignment = "let" designator "<-" expression
Assignment *
smpl_assignment (TokenNode **tn) {

	// new assgnmt, desig, result calloc'd here
	Assignment *a = init_assignment();
	assert_token_type_is(LET, tn,
											 ASSIGNMENT_NO_LET);
	next_token(tn);
	a->lhs = smpl_designator(tn);
	assert_token_type_is(LARROW, tn,
											 ASSIGNMENT_NO_LARROW);
	next_token(tn);
	a->rhs = smpl_expression(tn);
	return a;
}

// designator = ident{ "[" expression "]" }
Designator *
smpl_designator (TokenNode **tn) {

	// new des, ident, res_list calloc'd here
	Designator *d = init_designator();
	assert_token_type_is(IDENT, tn,
											 DESIGNATOR_NO_NAME_REFERENCE);
	// new ident calloc'd here TODO: double calloc?
	d->ident = build_ident((*tn)->tkn->raw);
	next_token(tn);
	while ( token_type_is(LBRACKET, tn) ) {

		next_token(tn);
		// new res list node calloc'd here
		push_res_list_data(&d->indices,
											 smpl_expression(tn));
		assert_token_type_is(RBRACKET, tn,
												 DESIGNATOR_NO_RBRACKET);
		next_token(tn);
	}
	return d;
}

// expression = term {("+" | "-") term}
union Result *
smpl_expression (TokenNode **tn) {

	// Nothing by the result union calloc'd here
	union Result *r = init_res();	
	r->bin_op->lhs = smpl_term(tn);
	while ( token_type_is(PLUS, tn) ||
					token_type_is(MINUS, tn) ) {
		
		r->bin_op->op = token_type_is(PLUS, tn) ? "+" : "-";
		next_token(tn);
		r->bin_op->rhs = smpl_term(tn);
	}
	return r;
}

// term = factor {("*" | "/") factor}
union Result *
smpl_term (TokenNode **tn) {

	// Nothing by the result union calloc'd here
	union Result *r = init_res();
	r->bin_op->lhs = smpl_factor(tn);
	while ( token_type_is(ASTERISK, tn) ||
					token_type_is(SLASH, tn) ) {
		
		r->bin_op->op =
			token_type_is(ASTERISK, tn) ? "*" : "/";
		next_token(tn);
		r->bin_op->rhs = smpl_factor(tn);
	}
	return r;
}

// factor = designator | number | "(" expression ")" |
// funcCall
// NOTE: only "non-void" fns may be called here.
union Result *
smpl_factor (TokenNode **tn) {

	// Nothing by the result union calloc'd here
	union Result *r = init_res();
	if ( token_type_is(IDENT, tn) ) {

		r->des = smpl_designator(tn);
	} else if ( token_type_is(NUMBER, tn) ) {

		r->num = build_num((*tn)->tkn->val);
		next_token(tn);

	} else if ( token_type_is(LPAREN, tn) ) {

		next_token(tn);
		r = smpl_expression(tn);
		assert_token_type_is(RPAREN, tn, FACTOR_NO_RPAREN);
		next_token(tn);
	} else if ( token_type_is(CALL, tn) ) {

		r->func_call = smpl_func_call(tn);
	} else { psr_err(tn, FACTOR_UNK_TOKEN_FOUND); }
	return r;
}

// funcCall =
// "call" ident [ "(" [expression
// { "," expression } ] ")" ]
// fns without params don't need "()" but can have them!
FuncCall *
smpl_func_call (TokenNode **tn) {

	// ident and result list calloc'd here
	FuncCall *fc = init_func_call();
	assert_token_type_is(CALL, tn, FUNC_CALL_NO_CALL);
	next_token(tn);
	assert_token_type_is(IDENT, tn,
											 FUNC_CALL_NAME_REFERENCE);
	fc->ident = build_ident((*tn)->tkn->raw);
	next_token(tn);
	// No error b/c "(" is optional
	if ( token_type_is(LPAREN, tn) ) {

		next_token(tn);
		// new res list node calloc'd here
		// TODO: will arg_exprs->head exist due to the union?
		push_res_list_data(&fc->arg_exprs,
											 smpl_expression(tn));
		while ( token_type_is(COMMA, tn) ) {

			// new res list node calloc'd here
			push_res_list_data(&fc->arg_exprs,
												 smpl_expression(tn));
		}
		assert_token_type_is(RPAREN, tn,
												 FUNC_CALL_NO_RPAREN);
		next_token(tn);
	}
	return fc;
}

// relation = expression relOp expression
BinOp *
smpl_relation (TokenNode **tn) {

	// res str and res all calloc'd here
	BinOp *bo = init_bin_op();
	bo->lhs = smpl_expression(tn);
	if ( token_type_is(OP_INEQ, tn) ||
			 token_type_is(OP_EQ, tn) ||
			 token_type_is(OP_LT, tn) ||
			 token_type_is(OP_LE, tn) ||
			 token_type_is(OP_GT, tn) ||
			 token_type_is(OP_GE, tn) ) {

		// the op can only be two chars
		strncpy(bo->op, (*tn)->tkn->raw, 2);
		next_token(tn);
	} else { psr_err(tn, RELATION_NO_RELOP); }
	
	bo->rhs = smpl_expression(tn);
	return bo;
}

// ifStatement =
// "if" relation "then"
// statSequence [ "else" statSequence ] "fi"
IfStmt *
smpl_if_statement (TokenNode **tn) {

	// binop and 2 stmt lists calloc'd here
	IfStmt *is = init_if_stmt();
	assert_token_type_is(IF, tn, IF_STATEMENT_NO_IF);
	next_token(tn);
	//TODO: check for double calloc?
	is->condition = smpl_relation(tn);
	assert_token_type_is(THEN, tn, IF_STATEMENT_NO_THEN);
	next_token(tn);
	//TODO: check for double calloc?
	is->then_stmts = smpl_stat_sequence(tn);
	// No error "else" is optional
	if ( token_type_is(ELSE, tn) ) {

		next_token(tn);
		// new sln calloc'd here
		is->else_stmts = smpl_stat_sequence(tn);
	}
	assert_token_type_is(FI, tn, IF_STATEMENT_NO_FI);
	next_token(tn);
	return is;
}

// whileStatement = "while" relation "do"
// StatSequence "od"
WhileStmt *
smpl_while_statement (TokenNode **tn) {

	// binop and sl calloc'd here
	WhileStmt *ws = init_while_stmt();
	assert_token_type_is(WHILE, tn,
											 WHILE_STATEMENT_NO_WHILE);
	//TODO: check for double calloc?
	ws->condition = smpl_relation(tn);
	assert_token_type_is(DO, tn, WHILE_STATEMENT_NO_DO);
	next_token(tn);
	//TODO: check for double calloc?
	ws->do_stmts = smpl_stat_sequence(tn);
	assert_token_type_is(OD, tn,
											 WHILE_STATEMENT_NO_OD);
	next_token(tn);
	return ws;
}

// returnStatement = "return" [ expression ]
ReturnStmt *
smpl_return_statement (TokenNode **tn) {

	// res calloc'd here
	ReturnStmt *rs = init_return_stmt();
	assert_token_type_is(RETURN, tn,
											 RETURN_STATEMENT_NO_RETURN);
	next_token(tn);
	//TODO: check for double calloc?
	rs->ret_val = smpl_expression(tn);
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
psr_err (TokenNode **tn,
				 ParserError e) {

	printf("Error found on line %i\n", (*tn)->tkn->line);
	perror(parser_error_table[e]);
	exit(1);
}

bool
token_type_is (TokenType type,
							 TokenNode **tn) {

	if ( (*tn)->tkn->type == type ) {
		return true;
	}
	return false;
}

void
assert_token_type_is (TokenType type,
											TokenNode **tn,
											ParserError e) {

	if ( (*tn)->tkn->type == type ) {
		return;
	}
	psr_err(tn, e);
}
