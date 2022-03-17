#include "../hdr/while_stmt.h"

WhileStmt *
init_while_stmt () {

	WhileStmt *ws = calloc(1, sizeof(WhileStmt));
	ws->condition = init_bin_op();
	ws->do_stmts  = init_stmt_list();
	return ws;
}
