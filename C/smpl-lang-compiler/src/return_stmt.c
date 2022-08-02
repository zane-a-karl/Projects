#include "../hdr/return_stmt.h"

struct ReturnStmt *
new_return_stmt ()
{
	struct ReturnStmt *rs = calloc(1, sizeof(struct ReturnStmt));
	rs->ret_val           = NULL;
	return rs;
}
