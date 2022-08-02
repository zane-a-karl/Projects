#ifndef _AST_H_
#define _AST_H_

#include "../hdr/computation.h"
#include "../hdr/var_decl_list.h"
#include "../hdr/number_list.h"
#include "../hdr/func_decl_list.h"
#include "../hdr/identifier_list.h"
#include "../hdr/assignment.h"
#include "../hdr/designator.h"
//#include "../hdr/bin_op.h"
#include "../hdr/func_call.h"
#include "../hdr/if_stmt.h"
#include "../hdr/while_stmt.h"
#include "../hdr/return_stmt.h"

#include <graphviz/cgraph.h>

struct AstNodeList;

struct AstNode {
	Anode_t *anode;
	union {
		struct Computation *computation;
		struct VarDecl *var_decl;
		struct Number *num;
		struct FuncDecl *func_decl;
		struct Identifier *ident;
		struct Assignment *ass;
		struct Designator *des;
		struct BinOp *bin_op;
		struct FuncCall *func_call;
		struct IfStmt *if_stmt;
		struct WhileStmt *while_stmt;
		struct ReturnStmt *ret_stmt;
	};
	struct AstNodeList *children;
};

struct AstNodeList {
	struct AstNode *head;
}

struct Ast {
	Agraph_t *agraph;
	struct AstNode *root;
};

struct Ast *
new_ast ();

struct AstNode *
new_ast_node (Agraph_t *g,
							char *node_name);

/* void */
/* append_subtree (struct Ast **subtree, */
/* 								struct Ast **ast); */

/* void */
/* push_basic_block (BasicBlock *bb, */
/* 									struct Ast **ast); */

/* void */
/* free_subtree (struct Ast **subtree); */

#endif//_AST_H_
