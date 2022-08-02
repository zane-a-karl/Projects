#include "../hdr/parser.h"

// Global symbol lookup table.
//extern VarTable *vt;

// Tkns that signal a stmt beginning
const enum TokenType stmt_terminals[] = {LET,
																				 CALL,
																				 IF,
																				 WHILE,
																				 RETURN};
const int num_stmt_terminals =
	sizeof(stmt_terminals)/sizeof(enum TokenType);

const char *psr_sectors[] = {
	"COMPUTATION",
	"VAR_DECL",
	"TYPE_DECL",
	"FUNC_DECL",
	"FORMAL_PARAM",
	"FUNC_BODY",
	"STAT_SEQ",
	"STMT",
	"ASSIGNMENT",
	"DESIGNATOR",
	"EXPRESSION",
	"TERM",
	"FACTOR",
	"FUNC_CALL",
	"IF_STMT",
	"RELATION",
	"WHILE_STMT",
	"RETURN_STMT"
};

extern const char *tkn_types[];

struct Parser *
new_parser (char *filename)
{
	struct Parser *p   = calloc(1, sizeof(struct Parser));
	// Allocates heap memory to lexer
	p->lxr      = new_lexer(filename);
	p->curr_tkn = lex_next_tkn(p->lxr);
	/* p->fout = fopen("", "r"); */
	/* check_fopen(p->fout); */
	return p;
}

void
free_parser (struct Parser **p)
{
	free_lexer(&((*p)->lxr));
	// p->curr_tkn is free'd with the lexer
	/* fclose((*p)->fout); */
	free(*p);
}

bool
peek_tkn (enum TokenType t,
					struct Parser *p)
{
	return p->curr_tkn->tkn->type == t;
}

bool
peek_stmt_terminals (struct Parser *p)
{
	for (int i = 0; i < num_stmt_terminals; ++i) {
		if (p->curr_tkn->tkn->type == stmt_terminals[i])
			return true;
	}
	return false;
}

void
consume_tkn (enum TokenType t,
						 struct Parser *p,
						 enum ParserSector ps)
{
	if ( peek_tkn(t, p) ) {
		p->curr_tkn = lex_next_tkn(p->lxr);
	} else {
		throw_parser_error(t, p, ps);
	}
}

struct Ast *
parse (struct Parser *p)
{
	// ONLY ast and ast->agraph calloc'd here
	struct Ast *ast = new_ast();
	ast->root       = smpl_computation(p, ast);

	return ast;
}

// computation = "main" { varDecl } { funcDecl }
// "{" statSequence "}" "."
AstNode *
smpl_computation (struct Parser *p,
									struct Ast *ast)
{
	// ONLY c, vdl, and fdl calloc'd here
	struct Computation *c = new_computation();
	consume_tkn(MAIN, p, COMPUTATION);
	while ( peek_tkn(VAR, p) ||	peek_tkn(ARRAY, p) ) {
		push_vd(c->var_decls, smpl_var_decl(p));
	}
	while ( peek_tkn(FUNCTION, p) || peek_tkn(VOID, p) ) {
		push_fd(c->func_decls, smpl_func_decl(p));
	}
	consume_tkn(LBRACE, p, COMPUTATION);
	c->stmts = smpl_stat_sequence(p);
	consume_tkn(RBRACE, p, COMPUTATION);
	consume_tkn(PERIOD, p, COMPUTATION);

	struct AstNode *node;
	node = new_ast_node(ast->agraph, "computation");
	node->computation = c;
	return node;
}

// varDecl = typeDecl ident { "," ident } ";"
struct VarDecl *
smpl_var_decl (struct Parser *p)
{
	// ONLY vd and il calloc'd here
	struct VarDecl *vd = new_vd();
	vd->dimensions = smpl_type_decl(p);
	push_ident(vd->identifiers, smpl_ident(p, VAR_DECL));

	while ( peek_tkn(COMMA, p) ) {
		consume_tkn(COMMA, p, VAR_DECL);
		push_ident(vd->identifiers, smpl_ident(p, VAR_DECL));
	}
	consume_tkn(SEMICOLON, p, VAR_DECL);

	return vd;
}

// typeDecl = "var" |
// "array" "[" number "]" { "[" number "]"}
// Note: we only have to handle integer types and their
//  arrays because of this we can have the parser return
//  the list of dimensions of the identifier. For scalars
//  we would just  have an empty list, i.e.
//  list->head = NULL.
struct NumberList *
smpl_type_decl (struct Parser *p)
{
	// ONLY nl calloc'd here
	struct NumberList *dims = new_num_list();
	if ( peek_tkn(VAR, p) ) {
		consume_tkn(VAR, p, TYPE_DECL);
		dims->head = NULL; // i.e. a scalar

	} else if ( peek_tkn(ARRAY, p) ) {
		consume_tkn(ARRAY, p, TYPE_DECL);
		do {
			consume_tkn(LBRACKET, p, TYPE_DECL);
			push_num(dims, smpl_number(p, TYPE_DECL));
			consume_tkn(RBRACKET, p, TYPE_DECL);
		} while ( peek_tkn(LBRACKET, p) );

	} else {
		throw_parser_error(p->curr_tkn->tkn->type, p, TYPE_DECL);
	}

	return dims;
}

struct Number *
smpl_number (struct Parser *p,
						 enum ParserSector ps)
{
	// ONLY num calloc'd here
	struct Number *num = new_num();
	consume_tkn(NUMBER, p, ps);
	num->val = p->curr_tkn->tkn->val;
	return num;
}

struct Ident *
smpl_ident (struct Parser *p,
						enum ParserSector ps)
{
	// ONLY ident and name calloc'd here
	struct Ident *ident = new_ident();
	consume_tkn(IDENT, p, ps);
	for (int i = 0; i < MAX_VAR_NAME_LEN; ++i) {
		ident->name[i] = p->curr_tkn->tkn->raw[i];
	}
	return ident;
}

// funcDecl =
// [ "void" ] "function"
// ident formalParam ";" funcBody ";"
struct FuncDecl *
smpl_func_decl (struct Parser *p)
{
	// ONLY fd calloc'd here
	struct FuncDecl *fd = new_fd();
	if ( peek_tkn(VOID, p) ) {
		consume_tkn(VOID, p, FUNC_DECL);
		fd->is_void = true;
	}
	consume_tkn(FUNCTION, p, FUNC_DECL);
	fd->name    = smpl_ident(p, FUNC_DECL);
	fd->params  = smpl_formal_param(p);
	consume_tkn(SEMICOLON, p, FUNC_DECL);
	fd->body    = smpl_func_body(p);
	consume_tkn(SEMICOLON, p, FUNC_DECL);

	return fd;
}

// formalParam = "(" [ident { "," ident }] ")"
struct IdentList *
smpl_formal_param (struct Parser *p)
{
	// ONLY il calloc'd here
	struct IdentList *params = new_ident_list();
	consume_tkn(LPAREN, p, FORMAL_PARAM);
	if ( peek_tkn(IDENT, p) ) {
		push_ident(params, smpl_ident(p, FORMAL_PARAM));
		while ( peek_tkn(COMMA, p) ) {
			consume_tkn(COMMA, p, FORMAL_PARAM);
			push_ident(params, smpl_ident(p, FORMAL_PARAM));
		} // endwhile
	} // endif
	consume_tkn(RPAREN, p, FORMAL_PARAM);

	return params;
}

// funcBody = { varDecl } "{" [ statSequence ] "}"
struct FuncBody *
smpl_func_body (struct Parser *p)
{
	// ONLY fb and vdl calloc'd here
	struct FuncBody *fb = new_func_body();
	while ( peek_tkn(VAR, p) ||
					peek_tkn(ARRAY, p) ) {

		// new vdln calloc'd here
		push_vd(fb->local_vars, smpl_var_decl(p));
	}
	consume_tkn(LBRACE, p, FUNC_BODY);
	fb->stmts = smpl_stat_sequence(p);
	consume_tkn(RBRACE, p, FUNC_BODY);

	return fb;
}

// statSequence = statement { ";" statement } [ ";" ]
struct StmtList *
smpl_stat_sequence (struct Parser *p)
{
	// ONLY sl calloc'd here
	struct StmtList *sl = new_sl();
	push_stmt(sl, smpl_statement(p));
	while ( peek_tkn(SEMICOLON, p) ) {

		consume_tkn(SEMICOLON, p, STAT_SEQ);
		if ( peek_stmt_terminals(p) ) {
			push_stmt(sl, smpl_statement(p));
		}
	}
	return sl;
}

// statement =
// assignment | "void" funcCall | ifStatement |
// whileStatement | returnStatement
struct Stmt *
smpl_statement (struct Parser *p)
{
	// ONLY Stmt calloc'd here
	struct Stmt *s = new_stmt();
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
		throw_parser_error(p->curr_tkn->tkn->type, p, STMT);
	}
	return s;
}

// assignment = "let" designator "<-" expression
struct Assignment *
smpl_assignment (struct Parser *p)
{
	// ONLY assignment calloc'd here
	struct Assignment *a = new_assignment();
	consume_tkn(LET, p,	ASSIGNMENT);
	a->lhs = smpl_designator(p);
	consume_tkn(LARROW, p, ASSIGNMENT);
	a->rhs = smpl_expression(p);
	
	return a;
}

// designator = ident{ "[" expression "]" }
struct Designator *
smpl_designator (struct Parser *p)
{
	// ONLY designator and result list calloc'd here
	struct Designator *d = new_designator();
	d->ident = smpl_ident(p, DESIGNATOR);
	while ( peek_tkn(LBRACKET, p) ) {

		consume_tkn(LBRACKET, p, DESIGNATOR);
		push_result(d->indices, smpl_expression(p));
		consume_tkn(RBRACKET, p, DESIGNATOR);
	}
	/////////////////////////////////////////////////////////
	// CHECK: CHECK for d->indices==NULL ?? Y/N??     ///// /
	/////////////////////////////////////////////////////////
	return d;
}

// expression = term {("+" | "-") term}
struct Result *
smpl_expression (struct Parser *p)
{
	// ONLY the `result` is calloc'd here
	struct Result *r = new_res();
	r->bin_op = new_bin_op();
	r->bin_op->lhs = smpl_term(p);
	while ( peek_tkn(PLUS, p) ||
					peek_tkn(MINUS, p) ) {

		r->bin_op->op = peek_tkn(PLUS, p) ? "+" : "-";
		if ( peek_tkn(PLUS, p) ) {
			consume_tkn(PLUS, p, EXPRESSION);
		} else {
			consume_tkn(MINUS, p, EXPRESSION);
		}
		r->bin_op->rhs = smpl_term(p);
	}
	return r;
}

// term = factor {("*" | "/") factor}
struct Result *
smpl_term (struct Parser *p)
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
			consume_tkn(ASTERISK, p, TERM);
		} else {
			consume_tkn(SLASH, p, TERM);
		}
		r->bin_op->rhs = smpl_factor(p);
	}
	return r;
}

// factor = designator | number | "(" expression ")" |
// funcCall
// NOTE: only "non-void" fns may be called here.
struct Result *
smpl_factor (struct Parser *p)
{
	// ONLY the `result`, its `binop`, and the binop's `op`
	// are calloc'd here
	struct Result *r = new_res();
	if ( peek_tkn(IDENT, p) ) {

		r->des = smpl_designator(p);
	} else if ( peek_tkn(NUMBER, p) ) {

		r->num = smpl_number(p, FACTOR);
	} else if ( peek_tkn(LPAREN, p) ) {

		consume_tkn(LPAREN, p, FACTOR);
		r = smpl_expression(p);
		consume_tkn(RPAREN, p, FACTOR);
	} else if ( peek_tkn(CALL, p) ) {

		r->func_call = smpl_func_call(p);
	} else {
		throw_parser_error(p->curr_tkn->tkn->type, p, FACTOR);
	}

	return r;
}

// funcCall =
// "call" ident [ "(" [expression
// { "," expression } ] ")" ]
// fns without params don't need "()" but can have them!
struct FuncCall *
smpl_func_call (struct Parser *p)
{
	// ONLY fc and fc->args calloc'd here
	struct FuncCall *fc = new_func_call();
	consume_tkn(CALL, p, FUNC_CALL);
	fc->name = smpl_ident(p, FUNC_CALL);
	if ( peek_tkn(LPAREN, p) ) {

		consume_tkn(LPAREN, p, FUNC_CALL);
		push_result(fc->args, smpl_expression(p));
		while ( peek_tkn(COMMA, p) ) {

			consume_tkn(COMMA, p, FUNC_CALL);
			push_result(fc->args, smpl_expression(p));
		}
		consume_tkn(RPAREN, p, FUNC_CALL);
	}
	return fc;
}

// relation = expression relOp expression
struct BinOp *
smpl_relation (struct Parser *p)
{
	// ONLY bo and bo->op calloc'd here
	struct BinOp *bo = new_bin_op();
	bo->lhs = smpl_expression(p);
	if ( peek_tkn(OP_INEQ, p) ||
			 peek_tkn(OP_EQ, p) ||
			 peek_tkn(OP_LT, p) ||
			 peek_tkn(OP_LE, p) ||
			 peek_tkn(OP_GT, p) ||
			 peek_tkn(OP_GE, p) ) {

		consume_tkn(p->curr_tkn->tkn->type, p, RELATION);
		// the op can only be two chars
		strncpy(bo->op, p->curr_tkn->tkn->raw, 2);
	}
	bo->rhs = smpl_expression(p);

	return bo;
}

// ifStatement =
// "if" relation "then"
// statSequence [ "else" statSequence ] "fi"
struct IfStmt *
smpl_if_statement (struct Parser *p)
{
	// ONLY ifstmt `is` calloc'd here
	struct IfStmt *is = new_if_stmt();
	consume_tkn(IF, p, IF_STMT);
	is->condition = smpl_relation(p);
	consume_tkn(THEN, p, IF_STMT);
	is->then_stmts = smpl_stat_sequence(p);
	if ( peek_tkn(ELSE, p) ) {

		consume_tkn(ELSE, p, IF_STMT);
		is->else_stmts = smpl_stat_sequence(p);
	}
	consume_tkn(FI, p, IF_STMT);

	return is;
}

// whileStatement = "while" relation "do"
// StatSequence "od"
struct WhileStmt *
smpl_while_statement (struct Parser *p)
{
	// ONLY whilestmt `ws` calloc'd here
	struct WhileStmt *ws = new_while_stmt();
	consume_tkn(WHILE, p, WHILE_STMT);
	ws->condition = smpl_relation(p);
	consume_tkn(DO, p, WHILE_STMT);
	ws->do_stmts  = smpl_stat_sequence(p);
	consume_tkn(OD, p, WHILE_STMT);

	return ws;
}

// returnStatement = "return" [ expression ]
struct ReturnStmt *
smpl_return_statement (struct Parser *p)
{
	// ONLY returnstmt `rs` calloc'd here
	struct ReturnStmt *rs = new_return_stmt();
	consume_tkn(RETURN, p, RETURN_STMT);
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
throw_parser_error (enum TokenType t,
										struct Parser *p,
										enum ParserSector ps)
{
	printf("Parser Error: ");
	printf("On (line:%d,col:%d) ", p->lxr->line, p->lxr->col);
	printf("expected '%s' but found '%s' ",
				 tkn_types[t],
				 p->curr_tkn->tkn->raw);
	printf("within '%s'", psr_sectors[ps]);
	exit(1);
}
