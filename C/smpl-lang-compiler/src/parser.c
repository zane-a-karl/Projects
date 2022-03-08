#include "../hdr/parser.h"

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

Ast *
parse (Parser *p) {

	Ast *ast = (Ast *)calloc(1, sizeof(Ast));
	BasicBlock *bb;
	Instruction *instr;
	// Check for the "computation"
	smpl_computation(p->tl->head);
	// TODO: Think about how "head" here
	//replaces "sym" from the lecture notes. Is "i" global
	// enough? I originally was passing the whole parser into
	// each function can I get away with only the next TokenNode
	// in the list? Should I take the tokenlist out of the
	// parser? or should I put more stuff in the parser? Do
	// I still need a "Result" struct or somthing like it?

	printf("Made it through parsing\n");
	return ast;
}

// computation =
// "main" { varDecl } { funcDecl } "{" statSequence "}" "."
void
smpl_computation (TokenNode *tn) {

	// Check for "main"
	if (tn->tkn->type == MAIN) {
		tn = tn->next;

		// Loop to handle var_decl repetition
		while (tn->tkn->type == VAR || tn->tkn->type == ARRAY) {
			tn = smpl_var_decl(tn);
		}

		// Loop to handle func_decl repetition
		while (tn->tkn->type == FUNCTION) {
			tn = smpl_func_decl(tn); // DOING THIS RIGHT NOW
		}

		// Check for "{"
		if (tn->tkn->type == LBRACE) {
			tn = tn->next;
			tn = smpl_stat_sequence(tn);
			// Check for "}"
			if (tn->tkn->type == RBRACE) {
				tn = tn->next;
			} else { psr_err(tn->tkn->line, COMPUTATION_NO_RBRACE); }
		} else { psr_err(tn->tkn->line, COMPUTATION_NO_LBRACE); }
	} else { psr_err(tn->tkn->line, COMPUTATION_NO_MAIN); }

	// Check for "."
	if (tn->tkn->type == PERIOD) {
		tn = tn->next;
	} else { psr_err(tn->tkn->line, COMPUTATION_NO_PERIOD); }
}

// varDecl = typeDecl ident { "," ident } ";"
TokenNode *
smpl_var_decl (TokenNode *tn) {

	tn = smpl_type_decl(tn);
	// Check "ident"
	// TODO: make this a util function b/c it's repeated
	if (tn->tkn->type == IDENT) {
		// TODO: add to hash table and registers
		printf("'var/arr %s' added to BB\n", tn->tkn->raw_tkn);
		
		tn = tn->next;

		// Check "{',' ident}"
		while (tn->tkn->type == COMMA) {
			tn = tn->next;
			if (tn->tkn->type == IDENT) {
				// TODO: add to hash table and registers
				printf("'var/arr %s' added to BB\n", tn->tkn->raw_tkn);
				
				tn = tn->next;
			} else { psr_err(tn->tkn->line, VAR_DECL_NO_VAR_NAME); }
		}
	} else { psr_err(tn->tkn->line, VAR_DECL_NO_VAR_NAME); }

	// Check ";"
	if (tn->tkn->type == SEMICOLON) {
		tn = tn->next;
	} else { psr_err(tn->tkn->line, VAR_DECL_NO_SEMICOLON); }

	return tn;
}

// typeDecl = "var" | "array" "[" number "]" { "[" number "]" }
TokenNode *
smpl_type_decl (TokenNode *tn) {

	// Check "var"
	if (tn->tkn->type == VAR) {
		return tn->next; // Do we need to do anything else here?

		// Check "array"
	} else if (tn->tkn->type == ARRAY) {
		tn = tn->next;
		do {
			// Check "["
			if (tn->tkn->type == LBRACKET) {
				tn = tn->next;
				// Check "number"
				if (tn->tkn->type == NUMBER) {
					// TODO: ADD TO BASICBLOCK
					tn = tn->next;
					// Check "]"
					if (tn->tkn->type == RBRACKET) {
						tn = tn->next;
					} else { psr_err(tn->tkn->line, TYPE_DECL_NO_RBRACKET); }
				} else { psr_err(tn->tkn->line, TYPE_DECL_NO_ARRAY_SIZE); }
			} else { psr_err(tn->tkn->line, TYPE_DECL_NO_LBRACKET); }

		} while (tn->tkn->type == LBRACKET);
		return tn; // Don't move foward b/c we already have!

	} else {
		return tn; // No error b/c var_decl > type_decl is optional
	}
}

// funcDecl =
// [ "void" ] "function" ident formalParam ";" funcBody ";"
TokenNode *
smpl_func_decl (TokenNode *tn) {

	// Check optional "void"
	if (tn->tkn->type == VOID) {
		tn = tn->next; //TODO: this changes something in registers?
	} // No error because "void" is optional

	// Check "function"
	if (tn->tkn->type == FUNCTION) {
		tn = tn->next;
		// Check ident
		if (tn->tkn->type == IDENT) {
			// TODO: add to BASICBLOCK
			printf("'fn stack for %s' added to BB\n", tn->tkn->raw_tkn);
			
			tn = tn->next;
			// Check smpl_formal_param
			tn = smpl_formal_param(tn);
			// Check ";"
			if (tn->tkn->type == SEMICOLON ) {
				tn = tn->next;
				// Check smpl_func_body
				tn = smpl_func_body(tn);
				// Check ";"
				if (tn->tkn->type == SEMICOLON ) {
					tn = tn->next;
				} else { psr_err(tn->tkn->line, FUNC_DECL_NO_SEMICOLON); }
			} else { psr_err(tn->tkn->line, FUNC_DECL_NO_SEMICOLON); }
		} else { psr_err(tn->tkn->line, FUNC_DECL_NO_FUNC_NAME);	}
	} else { psr_err(tn->tkn->line, FUNC_DECL_NO_FUNCTION); }

	return tn;
}

// formalParam = "(" [ident { "," ident }] ")"
TokenNode *
smpl_formal_param (TokenNode *tn) {

	// Check "("
	if (tn->tkn->type == LPAREN) {
		tn = tn->next;
		// Check optional [ident { "," ident }]
		// TODO: make this a util function b/c it's repeated
		// don't forget differing err types!
		if (tn->tkn->type == IDENT) {
			// TODO: add to hash table and registers
			tn = tn->next;

			// Check "{',' ident}"
			while (tn->tkn->type == COMMA) {
				tn = tn->next;
				if (tn->tkn->type == IDENT) {
					// TODO: add to hash table and registers
					tn = tn->next;
				} else { psr_err(tn->tkn->line, FORMAL_PARAM_NO_VAR_NAME); }
			}
		} // No error b/c param's are optional

		// Check ")"
		if (tn->tkn->type == RPAREN) {
			tn = tn->next;
		} else { psr_err(tn->tkn->line, FORMAL_PARAM_NO_RPAREN); }
	} else { psr_err(tn->tkn->line, FORMAL_PARAM_NO_LPAREN); }

	return tn;
}

// funcBody = { varDecl } "{" [ statSequence ] "}"
TokenNode *
smpl_func_body (TokenNode *tn) {

	// Loop to handle var_decl repetition
	while (tn->tkn->type == VAR || tn->tkn->type == ARRAY) {
		tn = smpl_var_decl(tn);
	}
	// Check "{"
	if (tn->tkn->type == LBRACE) {
		// Check optional [ statSequence ]
		tn = tn->next;
		tn = smpl_stat_sequence(tn); // Stmt can be empty
		// Check "{"
		if (tn->tkn->type == RBRACE) {
			tn = tn->next;
		} else { psr_err(tn->tkn->line, FUNC_BODY_NO_RBRACE); }
	} else { psr_err(tn->tkn->line, FUNC_BODY_NO_LBRACE); }

	return tn;
}

// statSequence = statement { ";" statement } [ ";" ]
TokenNode *
smpl_stat_sequence (TokenNode *tn) {

	tn = smpl_statement(tn);
	// Check repetitive { ";" statement }
	while (tn->tkn->type == SEMICOLON) {
		tn = tn->next;
		tn = smpl_statement(tn);
	}
	// Check optional [ ";" ]
	if (tn->tkn->type == SEMICOLON) {
		tn = tn->next;
	} // No error b/c terminating ";"s are optional

	return tn;
}

// statement =
// assignment | "void" funcCall | ifStatement |
// whileStatement | returnStatement
TokenNode *
smpl_statement (TokenNode *tn) {

	//Check assignment
	if (tn->tkn->type == LET) {
		tn = smpl_assignment(tn);

		//Check "void" func_call
		// TODO: how to make sure the function is VOID?
	} else if (tn->tkn->type == CALL) {
		tn = smpl_func_call(tn);

		//Check if_statement
	} else if (tn->tkn->type == IF) {
		tn = smpl_if_statement(tn);

		//Check while_statement
	} else if (tn->tkn->type == WHILE) {
		tn = smpl_while_statement(tn);

		//Check return_statement
	} else if (tn->tkn->type == RETURN) {
		tn = smpl_return_statement(tn);
	}

	return tn;
}

// assignment = "let" designator "<-" expression
TokenNode *
smpl_assignment (TokenNode *tn) {

	// Check "let"
	if (tn->tkn->type == LET) {
		tn = tn->next;
		// Check designator
		tn = smpl_designator(tn);
		// Check "<-"
		if (tn->tkn->type == LARROW) {
			tn = tn->next;
			tn = smpl_expression(tn);
		} else { psr_err(tn->tkn->line, ASSIGNMENT_NO_LARROW); }
	} else { psr_err(tn->tkn->line, ASSIGNMENT_NO_LET); }

	return tn;
}

// designator = ident{ "[" expression "]" }
TokenNode *
smpl_designator (TokenNode *tn) {

	// Check ident
	if (tn->tkn->type == IDENT) {
		// TODO: add to basic block and registers
		printf("'var/arr %s' added to hash tbl\n", tn->tkn->raw_tkn);
		
		tn = tn->next;
		// Check repetitive { "[" expression "]" }
		// TODO: ask proof if "[" and "]" are optional
		while (tn->tkn->type == LBRACKET) {
			tn = tn->next;
			tn = smpl_expression(tn);
			// TODO: access correct register and/or basic block
			if (tn->tkn->type == RBRACKET) {
				tn = tn->next;
			} else { psr_err(tn->tkn->line, DESIGNATOR_NO_RBRACKET); }
		}
	} else { psr_err(tn->tkn->line, DESIGNATOR_NO_NAME_REFERENCE); }

	return tn;
}

// expression = term {("+" | "-") term}
TokenNode *
smpl_expression (TokenNode *tn) {
	tn = smpl_term(tn); // this was 'x'
	while (tn->tkn->type == PLUS || tn->tkn->type == MINUS) {
		printf("'%s instr' added to BB\n", tn->tkn->raw_tkn);
		tn = tn->next;
		tn = smpl_term(tn); // this was 'y'
		// TODO: add "ADD" || "MINUS" basic block
	}
	return tn; // was 'x'
}

// term = factor { ("*" | "/") factor}
TokenNode *
smpl_term (TokenNode *tn) {
	tn = smpl_factor(tn); // this was 'x'
	while (tn->tkn->type == ASTERISK || tn->tkn->type == SLASH) {
		printf("'%s instr' added to BB\n", tn->tkn->raw_tkn);
		tn = tn->next;
		tn = smpl_factor(tn);// this was 'y'
		// TODO: add "MUL" || "DIV" basic block
	}
	return tn; // was 'x'
}

// factor = designator | number | "(" expression ")" | funcCall
// NOTE: only "non-void" fns may be called here.
TokenNode *
smpl_factor (TokenNode *tn) {

	// Check designator
	if (tn->tkn->type == IDENT) {
		tn = smpl_designator(tn);

		// Check number
	} else if (tn->tkn->type == NUMBER) {
		// TODO: store or access the number in hash table or regs
		printf("'const %s' added to BB\n", tn->tkn->raw_tkn);
		tn = tn->next;

		// Check "("
	} else if (tn->tkn->type == LPAREN) {
		tn = tn->next;
		tn = smpl_expression(tn); // was 'x'
		if (tn->tkn->type == RPAREN) {
			tn = tn->next;
		} else { psr_err(tn->tkn->line, TERM_NO_RPAREN);	}

		// Check 'non-void' func_call
		// ie just uses 'call' with no 'void' in front
		// TODO: might need to check that this function is somehow
		// void via a lookup()!
	} else if (tn->tkn->type == CALL) {
		tn = smpl_func_call(tn);
	}

	return tn; // was x
}

// funcCall =
// "call" ident [ "(" [expression { "," expression } ] ")" ]
// fns without params don't need "()" but can have them!
TokenNode *
smpl_func_call (TokenNode *tn) {

	// Check "call"
	if (tn->tkn->type == CALL) {
		tn = tn->next;
		// Check ident = fn name
		if (tn->tkn->type == IDENT) {
			// TODO: Access the function in registers/basic block
			tn = tn->next;
			// Check optional [ "(" ... ")" ]
			if (tn->tkn->type == LPAREN) {
				tn = tn->next;
				// Check optional [expression { "," expression } ]
				tn = smpl_expression(tn);
				// Check repetitive  { "," expression }
				while (tn->tkn->type == COMMA) {
					tn = smpl_expression(tn);
				}
				// Check ")", mandatory if there was a "("
				if (tn->tkn->type == RPAREN) {
					tn = tn->next;
				} else { psr_err(tn->tkn->line, FUNC_CALL_NO_RPAREN); }
			} // No error b/c "(" is optional
		} else { psr_err(tn->tkn->line, FUNC_CALL_NAME_REFERENCE); }
	} else { psr_err(tn->tkn->line, FUNC_CALL_NO_CALL); }

	return tn;
}

// ifStatement =
// "if" relation "then"
// statSequence [ "else" statSequence ] "fi"
TokenNode *
smpl_if_statement (TokenNode *tn) {

	// Check "if"
	if (tn->tkn->type == IF) {
		printf("'%s block' added to BB\n", tn->tkn->raw_tkn);
		tn = tn->next;
		// Check smpl_relation
		tn = smpl_relation(tn);
		// Check "then"
		if (tn->tkn->type == THEN) {
			printf("'%s block' added to BB\n", tn->tkn->raw_tkn);
			tn = tn->next;
			// TODO: make sure stat_sequence adds to a basic block
			tn = smpl_stat_sequence(tn);
			// Check optional [ "else" statSequence ]
			if (tn->tkn->type == ELSE) {
				printf("'%s block' added to BB\n", tn->tkn->raw_tkn);
				tn = tn->next;
				// TODO: make sure stat_sequence adds to a basic block
				tn = smpl_stat_sequence(tn);
			}
			// Check "fi"
			if (tn->tkn->type == FI) {
				printf("'JOIN = %s block' added to BB\n", tn->tkn->raw_tkn);
				tn = tn->next;
			} else { psr_err(tn->tkn->line, IF_STATEMENT_NO_FI); }
		} else { psr_err(tn->tkn->line, IF_STATEMENT_NO_THEN); }
	}	else { psr_err(tn->tkn->line, IF_STATEMENT_NO_IF); }

	return tn;
}

// relation = expression relOp expression
TokenNode *
smpl_relation (TokenNode *tn) {

	// Check expression
	tn = smpl_expression(tn);
	// Check relOp
	if (tn->tkn->type == OP_INEQ ||
			tn->tkn->type == OP_EQ ||
			tn->tkn->type == OP_LT ||
			tn->tkn->type == OP_LE ||
			tn->tkn->type == OP_GT ||
			tn->tkn->type == OP_GE) {
		// TODO: add correct instr to basic block
		printf("'%s relation instr' added to BB\n", tn->tkn->raw_tkn);
		tn = tn->next;
	} else { psr_err(tn->tkn->line, RELATION_NO_RELOP); }
	// Check expression
	tn = smpl_expression(tn);

	return tn;
}

// whileStatement = "while" relation "do" StatSequence "od"
TokenNode *
smpl_while_statement (TokenNode *tn) {

	// Check "while"
	if (tn->tkn->type == WHILE) {
		printf("'%s block' added to BB\n", tn->tkn->raw_tkn);		
		tn = tn->next;
		// Check relation
		tn = smpl_relation(tn);
		// Check "do"
		if (tn->tkn->type == DO) {
			tn = tn->next;
			// Check stat_sequence
			tn = smpl_stat_sequence(tn);
			// Check "od"
			if (tn->tkn->type == OD) {
				printf("'JOIN=Consequence=%s block' added to BB\n", tn->tkn->raw_tkn);		
				tn = tn->next;
			} else { psr_err(tn->tkn->line, WHILE_STATEMENT_NO_OD); }
		} else { psr_err(tn->tkn->line, WHILE_STATEMENT_NO_DO); }
	} else { psr_err(tn->tkn->line, WHILE_STATEMENT_NO_WHILE); }

	return tn;
}

// returnStatement = "return" [ expression ]
TokenNode *
smpl_return_statement (TokenNode *tn) {

	// Check "return"
	if (tn->tkn->type == RETURN) {
		printf("'clean up vars=%s block' added to BB\n", tn->tkn->raw_tkn);		
		tn = tn->next;
		// Check optional [ expression ]
		// optional means we can have an empty expression!
		// Check that this is the case!!!
		tn = smpl_expression(tn);
	} else { psr_err(tn->tkn->line, RETURN_STATEMENT_NO_RETURN); }

	return tn;
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
