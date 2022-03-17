#include "../hdr/if_stmt.h"

IfStmt *
init_if_stmt () {

	IfStmt *is     = calloc(1, sizeof(IfStmt));
	is->condition  = init_bin_op();
	is->then_stmts = init_sl();
	is->else_stmts = init_sl();
	return is;
}
