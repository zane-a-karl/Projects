#ifndef _AST_H_
#define _AST_H_

#include "../hdr/computation.h"
#include "../hdr/var_decl_list.h"
#include "../hdr/number_list.h"
#include "../hdr/func_decl_list.h"
#include "../hdr/identifier_list.h"
#include "../hdr/assignment.h"
#include "../hdr/designator.h"
#include "../hdr/bin_op.h"
#include "../hdr/func_call.h"
#include "../hdr/if_stmt.h"
#include "../hdr/while_stmt.h"
#include "../hdr/return_stmt.h"

typedef struct AstNode {
	union {
		Computation *comp;
		struct VarDecl *var_decl;
		struct Number *num;
		struct FuncDecl *func_decl;
		struct Identifier *ident;
		Assignment *ass;
		Designator *des;//formerly ArrayAccess *arr_acc;
		BinOp *bin_op;
		FuncCall *func_call;
		IfStmt *if_stmt;
		WhileStmt *while_stmt;
		ReturnStmt *ret_stmt;
	};
	//	struct AstNode *children;
} AstNode;

typedef struct Ast {
	AstNode *root;
} Ast;

Ast *
init_ast ();

/* void */
/* append_subtree (Ast **subtree, */
/* 								Ast **ast); */

/* void */
/* push_basic_block (BasicBlock *bb, */
/* 									Ast **ast); */

/* void */
/* free_subtree (Ast **subtree); */

#endif//_AST_H_
