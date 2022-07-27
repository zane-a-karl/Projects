#ifndef _IF_STMT_H_
#define _IF_STMT_H_

#include "../hdr/stmt_list.h"
#include "../hdr/bin_op.h"

typedef struct IfStmt {
	BinOp *condition;
	struct StmtList *then_stmts;
	struct StmtList *else_stmts;
} IfStmt;

IfStmt *
new_if_stmt ();

#endif//_IF_STMT_H_
