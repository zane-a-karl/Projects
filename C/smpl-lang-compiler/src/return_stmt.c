#include "../hdr/return_stmt.h"

struct ReturnStmt *
new_return_stmt ()
{
	struct ReturnStmt *rs = calloc(1, sizeof(struct ReturnStmt));
	rs->ret_val           = NULL;

	return rs;
}

struct ReturnStmt *
deep_copy_return_stmt (struct ReturnStmt *src)
{
	struct ReturnStmt *dst = new_return_stmt();
	dst->ret_val = deep_copy_ast_node(src->ret_val);

	return dst;
}
