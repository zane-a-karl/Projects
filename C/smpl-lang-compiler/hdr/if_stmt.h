#ifndef _IF_STMT_H_
#define _IF_STMT_H_

#include "../hdr/stmt_list.h"
//#include "../hdr/bin_op.h"
#include "../hdr/result_list.h"

struct IfStmt {
	struct BinOp *condition;
	struct StmtList *then_stmts;
	struct StmtList *else_stmts;
};

struct IfStmt *
new_if_stmt ();

#endif//_IF_STMT_H_
