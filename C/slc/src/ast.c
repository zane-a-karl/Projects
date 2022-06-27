#include "../hdr/ast.h"

Ast *
init_ast () {

	Ast *ast = (Ast *)calloc(1, sizeof(Ast));
	ast->root = (AstNode *)calloc(1, sizeof(AstNode));
	return ast;
}

