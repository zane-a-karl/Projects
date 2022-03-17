#include "../hdr/return_stmt.h"

ReturnStmt *
init_return_stmt () {

	ReturnStmt *rs = calloc(1, sizeof(ReturnStmt));
	rs->ret_val    = init_result_list();
	return rs;
}
