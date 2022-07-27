#include "../hdr/return_stmt.h"

ReturnStmt *
new_return_stmt ()
{
	ReturnStmt *rs = calloc(1, sizeof(ReturnStmt));
	rs->ret_val    = NULL;
	return rs;
}
