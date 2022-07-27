#include "../hdr/if_stmt.h"

IfStmt *
new_if_stmt ()
{
	IfStmt *is     = calloc(1, sizeof(IfStmt));
	is->condition  = NULL;
	is->then_stmts = NULL;
	is->else_stmts = NULL;
	return is;
}
