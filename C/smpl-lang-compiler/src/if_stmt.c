#include "../hdr/if_stmt.h"

struct IfStmt *
new_if_stmt ()
{
	struct IfStmt *is = calloc(1, sizeof(struct IfStmt));
	is->condition     = NULL;
	is->then_stmts    = NULL;
	is->else_stmts    = NULL;

	return is;
}

struct IfStmt *
deep_copy_if_stmt (struct IfStmt *src)
{
	struct IfStmt *dst = new_if_stmt();
	dst->condition = deep_copy_ast_node(src->condition);
	dst->then_stmts = deep_copy_ast_node_list(src->then_stmts);
	dst->else_stmts = deep_copy_ast_node_list(src->else_stmts);

	return dst;
}
