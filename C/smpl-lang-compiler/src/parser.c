// TODO: Have smpl functions output lists of "Instructions" that you can then stitch together
// You might need to add TYPEs for the Basic Blocks like Andre did for classes.



#include "../hdr/parser.h"

// Global symbol lookup table.
extern VarTable *vt;

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

	Computation *c = smpl_computation(&(p->tl->head));
	printf("Parsing Complete!\n");
	return ast;
}

// computation =
// "main" { varDecl } { funcDecl } "{" statSequence "}" "."
Computation *
smpl_computation (TokenNode **tn) {

	Computation *comp = init_computation();

	assert_token_type_is(MAIN, tn,
											 COMPUTATION_NO_MAIN);
	next_token(tn);
	while ( token_type_is(VAR, tn) ||
					token_type_is(ARRAY, tn) ) {

		smpl_var_decl(tn, &c->vdl);// vdl mutator
	}
	while ( token_type_is(FUNCTION, tn) ||
					token_type_is(VOID, tn) ) {

		smpl_func_decl(tn, &c->fdl);// fdl mutator
	}
	assert_token_type_is(LBRACE, tn,
											 COMPUTATION_NO_LBRACE);
	next_token(tn);
	smpl_stat_sequence(tn, &c->sl);// sl mutator
	assert_token_type_is(RBRACE, tn,
											 COMPUTATION_NO_RBRACE);
	next_token(tn);
	assert_token_type_is(PERIOD, tn,
											 COMPUTATION_NO_PERIOD);
	next_token(tn);
	return comp;
}

// varDecl = typeDecl ident { "," ident } ";"
void
smpl_var_decl (TokenNode **tn,
							 VarDeclsList **vdl) {

	VarDeclsListNode *vdln;
	vdln = init_var_decls_list_node();//calloc
	smpl_type_decl(tn,
								 &(vdln->dimens_list));//list mutator
	assert_token_type_is(IDENT, tn,
											 VAR_DECL_NO_VAR_NAME);
	push_str_list_data(&(vdln->idents_list),
										 (*tn)->tkn->raw);// calloc
	next_token(tn);
	while ( token_type_is(COMMA, tn) ) {
		next_token(tn);
		assert_token_type_is(IDENT, tn,
												 VAR_DECL_NO_VAR_NAME);
		push_str_list_data(&(vdln->idents_list),
											 (*tn)->tkn->raw);// calloc
		next_token(tn);
	}
	assert_token_type_is(SEMICOLON, tn,
											 VAR_DECL_NO_SEMICOLON);
	next_token(tn);
	push_var_decls_list_node(vdl, vdln);
}

// typeDecl = "var" | "array" "[" number "]" { "[" number "]"}
// Note: we only have to handle integer types and their arrays
// because of this we can have the parser return the list of
// dimensions of the identifier. For scalars we would just
// have an empty list, i.e. list->head = NULL.
void
smpl_type_decl (TokenNode **tn,
								IntList **dimens_list) {

	if ( token_type_is(VAR, tn) ) {

		(*dimens_list)->head = NULL; // i.e. a scalar
		next_token(tn);
	} else if ( token_type_is(ARRAY, tn) ) {

		next_token(tn);
		do {

			assert_token_type_is(LBRACKET, tn,
													 TYPE_DECL_NO_LBRACKET);
			next_token(tn);
			assert_token_type_is(NUMBER, tn,
													 TYPE_DECL_NO_ARRAY_SIZE);
			push_int_list_data(dimens_list,
												 (*tn)->tkn->val);//calloc
			next_token(tn);
			assert_token_type_is(RBRACKET, tn,
													 TYPE_DECL_NO_RBRACKET);
				next_token(tn);

		} while ( token_type_is(LBRACKET, tn) );

	} else { psr_err((*tn)->tkn->line,
									 TYPE_DECL_NO_TYPE);}
}

// funcDecl =
// [ "void" ] "function" ident formalParam ";" funcBody ";"
void
smpl_func_decl (TokenNode **tn,
								FuncDeclsList **fl) {

	FuncDeclsListNode *fdln;
	fdln = init_func_decls_list_node();//calloc
	// No error because "void" is optional
	if ( token_type_is(VOID, tn) ) {
		fdln->is_void = true;
		next_token(tn);
	}
	assert_token_type_is(FUNCTION, tn,
											 FUNC_DECL_NO_FUNCTION);
	next_token(tn);
	assert_token_type_is(IDENT, tn,
											 FUNC_DECL_NO_FUNC_NAME);
	set_fn_name(&fdln, tn);//fdln mutator
	next_token(tn);
	smpl_formal_param(tn, &fdln);//fdln mutator
	assert_token_type_is(SEMICOLON, tn,
											 FUNC_DECL_NO_SEMICOLON);
	next_token(tn);
	smpl_func_body(tn, &fdln);//fdln mutator
	assert_token_type_is(SEMICOLON, tn,
											 FUNC_DECL_NO_SEMICOLON);
	next_token(tn);
	push_func_decls_list_node(fdl, fdln);
}

// formalParam = "(" [ident { "," ident }] ")"
void
smpl_formal_param (TokenNode **tn,
									 FuncDeclsListNode **fdln) {

	assert_token_type_is(LPAREN, tn,
											 FORMAL_PARAM_NO_LPAREN);
	next_token(tn);
	if ( token_type_is(IDENT, tn) ) {

		push_str_list_data((*fdln)->param_idents,
											 (*tn)->tkn->raw);//fdln mutator
		next_token(tn);
		while ( token_type_is(COMMA, tn) ) {

			next_token(tn);
			assert_token_type_is(IDENT, tn,
													 FORMAL_PARAM_NO_VAR_NAME);
			push_str_list_data((*fdln)->param_idents,
												 (*tn)->tkn->raw);//fdln mutator
			next_token(tn);
		}
	} // No error b/c param's are optional
	assert_token_type_is(RPAREN, tn,
											 FORMAL_PARAM_NO_RPAREN);
	next_token(tn);
}

// funcBody = { varDecl } "{" [ statSequence ] "}"
void
smpl_func_body (TokenNode **tn,
								FuncDeclsListNode **fdln) {

	while ( token_type_is(VAR, tn) ||
					token_type_is(ARRAY, tn) ) {

		smpl_var_decl(tn,
									&(*fdln)->local_var_decls);//fdln mutator
	}
	assert_token_type_is(LBRACE, tn,
											 FUNC_BODY_NO_LBRACE);
	next_token(tn);
	// Stmt can be empty
	smpl_stat_sequence(tn, &(*fdln)->stmts);//fdln mutator
	assert_token_type_is(RBRACE, tn,
											 FUNC_BODY_NO_RBRACE);
	next_token(tn);
}

// statSequence = statement { ";" statement } [ ";" ]
void
smpl_stat_sequence (TokenNode **tn,
										StmtsList **sl) {

	smpl_statement(tn, sl);
	while ( token_type_is(SEMICOLON, tn) ) {

		next_token(tn);
		smpl_statement(tn, sl);
	}
	// No error b/c terminating ";"s are optional
	if ( token_type_is(SEMICOLON, tn) ) {

		next_token(tn);
	}
}

// statement =
// assignment | "void" funcCall | ifStatement |
// whileStatement | returnStatement
void
smpl_statement (TokenNode **tn,
								StmtsList **sl) {

	StmtsListNode *sln;
	sln = init_stmts_list_node();//calloc
	if ( token_type_is(LET, tn) ) {

		smpl_assignment(tn, &sln);//sln mutator
	} else if ( token_type_is(CALL, tn) ) {

		smpl_func_call(tn, &sln);//sln mutator
	} else if ( token_type_is(IF, tn) ) {

		smpl_if_statement(tn, &sln);//sln mutator
	} else if ( token_type_is(WHILE, tn) ) {

		smpl_while_statement(tn, &sln);//sln mutator
	} else if ( token_type_is(RETURN, tn) ) {

		smpl_return_statement(tn, &sln);//sln mutator
	}
	push_stmts_list_node(sl, sln);
}

// assignment = "let" designator "<-" expression
void
smpl_assignment (TokenNode **tn,
								 StmtsListNode **sln) {

	assert_token_type_is(LET, tn,
											 ASSIGNMENT_NO_LET);
	next_token(tn);
	smpl_designator(tn, &(*sln)->lhs);
	assert_token_type_is(LARROW, tn,
											 ASSIGNMENT_NO_LARROW);
	next_token(tn);
	smpl_expression(tn);
}

// designator = ident{ "[" expression "]" }
void
smpl_designator (TokenNode **tn,
								 Designator **des) {

	assert_token_type_is(IDENT, tn,
											 DESIGNATOR_NO_NAME_REFERENCE);
	// mutator: adds var name to des
	set_var_name(des, tn);
	next_token(tn);
	while ( token_type_is(LBRACKET, tn) ) {

		next_token(tn);
		// mutator: appends exprsn to sln->let_node
		push_result_list_data((*des)->indices,
													smpl_expression(tn));
		assert_token_type_is(RBRACKET, tn,
												 DESIGNATOR_NO_RBRACKET);
		next_token(tn);
	}
}

// expression = term {("+" | "-") term}
Result
smpl_expression (TokenNode **tn) {

	
	smpl_term(tn, bo->opa);// op->opa mutator
	while ( token_type_is(PLUS, tn) ||
					token_type_is(MINUS, tn) ) {
		
		bo->operator = token_type_is(PLUS, tn) ? '+' : '-';
		next_token(tn);
		smpl_term(tn, &op);// op->opb mutator
	}
	return bo->opa;
}

// term = factor { ("*" | "/") factor}
Operand
smpl_term (TokenNode **tn,
					 BinOp **bo) {

	smpl_factor(tn); // this was 'x'
	while ( token_type_is(ASTERISK, tn) ||
					token_type_is(SLASH, tn) ) {
		
		bo->operator = token_type_is(ASTERISK, tn) ? '*' : '/';
		next_token(tn);
		smpl_factor(tn);// this was 'y'
	}
	 // was 'x'
}

// factor = designator | number | "(" expression ")" | funcCall
// NOTE: only "non-void" fns may be called here.
void
smpl_factor (TokenNode **tn) {

	//	Result result;
	if ( token_type_is(IDENT, tn) ) {

		//		set_result(&result, VARIABLE, tn);
		smpl_designator(tn);
	} else if ( token_type_is(NUMBER, tn) ) {

		//		set_result(&result, CONSTANT, tn);
		next_token(tn);

	} else if ( token_type_is(LPAREN, tn) ) {

		next_token(tn);
		smpl_expression(tn); // was 'x'
		assert_token_type_is(RPAREN, tn, TERM_NO_RPAREN);
		next_token(tn);
		// Check 'non-void' func_call
		// ie just uses 'call' with no 'void' in front
		// TODO: might need to check that this function is
		// somehow void via a lookup()!?
	} else if ( token_type_is(CALL, tn) ) {

		smpl_func_call(tn);
	}
	 // was x
}

// funcCall =
// "call" ident [ "(" [expression { "," expression } ] ")" ]
// fns without params don't need "()" but can have them!
void
smpl_func_call (TokenNode **tn,
								StmtsListNode **sln) {

	assert_token_type_is(CALL, tn, FUNC_CALL_NO_CALL);
	next_token(tn);
	assert_token_type_is(IDENT, tn,
											 FUNC_CALL_NAME_REFERENCE);
	next_token(tn);
	// No error b/c "(" is optional
	if ( token_type_is(LPAREN, tn) ) {

		next_token(tn);
		smpl_expression(tn);
		while ( token_type_is(COMMA, tn) ) {

			smpl_expression(tn);
		}
		assert_token_type_is(RPAREN, tn,
												 FUNC_CALL_NO_RPAREN);
		next_token(tn);
	}
}

// ifStatement =
// "if" relation "then"
// statSequence [ "else" statSequence ] "fi"
void
smpl_if_statement (TokenNode **tn,
									 StmtsListNode **sln) {

	assert_token_type_is(IF, tn, IF_STATEMENT_NO_IF);
	next_token(tn);
	smpl_relation(tn);
	assert_token_type_is(THEN, tn, IF_STATEMENT_NO_THEN);
	next_token(tn);
	smpl_stat_sequence(tn);
	// No error "else" is optional
	if ( token_type_is(ELSE, tn) ) {

		next_token(tn);
		smpl_stat_sequence(tn);
	}
	assert_token_type_is(FI, tn, IF_STATEMENT_NO_FI);
	next_token(tn);
}

// relation = expression relOp expression
void
smpl_relation (TokenNode **tn) {

	smpl_expression(tn);
	if ( token_type_is(OP_INEQ, tn) ||
			 token_type_is(OP_EQ, tn) ||
			 token_type_is(OP_LT, tn) ||
			 token_type_is(OP_LE, tn) ||
			 token_type_is(OP_GT, tn) ||
			 token_type_is(OP_GE, tn) ) {

		next_token(tn);
	} else {psr_err((*tn)->tkn->line, RELATION_NO_RELOP);}
	smpl_expression(tn);
}

// whileStatement = "while" relation "do" StatSequence "od"
void
smpl_while_statement (TokenNode **tn,
											StmtsListNode **sln) {

	assert_token_type_is(WHILE, tn,
											 WHILE_STATEMENT_NO_WHILE);
	smpl_relation(tn);
	assert_token_type_is(DO, tn, WHILE_STATEMENT_NO_DO);
	next_token(tn);
	smpl_stat_sequence(tn);
	assert_token_type_is(OD, tn,
											 WHILE_STATEMENT_NO_OD);
	next_token(tn);
}

// returnStatement = "return" [ expression ]
void
smpl_return_statement (TokenNode **tn,
											 StmtsListNode **sln) {

	assert_token_type_is(RETURN, tn,
											 RETURN_STATEMENT_NO_RETURN);
	next_token(tn);
	smpl_expression(tn);
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
psr_err (int line,
				 ParserError e) {

	printf("Error found on line %i\n", line);
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
	psr_err((*tn)->tkn->line, e);
}
