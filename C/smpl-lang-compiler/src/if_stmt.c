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
