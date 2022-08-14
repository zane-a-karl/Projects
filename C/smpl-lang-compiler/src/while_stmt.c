#include "../hdr/while_stmt.h"

struct WhileStmt *
new_while_stmt ()
{
	struct WhileStmt *ws = calloc(1, sizeof(struct WhileStmt));
	ws->condition        = NULL;
	ws->do_stmts         = NULL;

	return ws;
}

struct WhileStmt *
deep_copy_while_stmt (struct WhileStmt *src)
{
	struct WhileStmt *dst = new_while_stmt();
	dst->condition = deep_copy_ast_node(src->condition);
	dst->do_stmts = deep_copy_ast_node_list(src->do_stmts);

	return dst;
}
