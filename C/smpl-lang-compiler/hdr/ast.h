#ifndef _AST_H_
#define _AST_H_

#include "../hdr/int_list.h"
#include "../hdr/str_list.h"

typedef struct AstNode {
	char *data;
	struct AstNode *children;
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
