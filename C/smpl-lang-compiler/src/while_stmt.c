#include "../hdr/while_stmt.h"

struct WhileStmt *
new_while_stmt ()
{
	struct WhileStmt *ws = calloc(1, sizeof(struct WhileStmt));
	ws->condition        = NULL;
	ws->do_stmts         = NULL;
	return ws;
}
