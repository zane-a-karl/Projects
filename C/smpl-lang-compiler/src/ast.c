#include "../hdr/ast.h"

Ast *
new_ast ()
{
	Ast *ast = (Ast *)calloc(1, sizeof(Ast));
	ast->root = (AstNode *)calloc(1, sizeof(AstNode));

	return ast;
}

/* void */
/* free_ast (Ast *ast) { */

/* 	free(ast->root); */
/* } */
