#include "../hdr/if_stmt.h"

IfStmt *
init_if_stmt () {

	IfStmt *is     = calloc(1, sizeof(IfStmt));
	is->condition  = init_bin_op();
	is->then_stmts = init_stmts_list();
	is->else_stmts = init_stmts_list();
	return is;
}
