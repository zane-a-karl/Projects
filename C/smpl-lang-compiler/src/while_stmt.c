#include "../hdr/while_stmt.h"

WhileStmt *
new_while_stmt ()
{
	WhileStmt *ws = calloc(1, sizeof(WhileStmt));
	ws->condition = NULL;
	ws->do_stmts  = NULL;
	return ws;
}
