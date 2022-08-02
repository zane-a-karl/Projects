#include "../hdr/ast.h"

struct Ast *
new_ast ()
{
	struct Ast *ast = calloc(1, sizeof(struct Ast));
	ast->agraph     = agopen("AST", Agdirected, NULL);
	ast->root       = NULL;

	return ast;
}

struct AstNode *
new_ast_node (Agraph_t *g,
							char *node_name)
{
	struct AstNode *node = calloc(1, sizeof(struct AstNode));
	node->anode = agnode(g, node_name, TRUE);

	return node;
}

/* void */
/* free_ast (struct Ast *ast) { */

/* 	free(ast->root); */
/* } */
