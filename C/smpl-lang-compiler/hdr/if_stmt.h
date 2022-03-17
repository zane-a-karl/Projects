#ifndef _IF_STMT_H_
#define _IF_STMT_H_

#include "../hdr/stmt_list.h"
#include "../hdr/bin_op.h"

typedef struct IfStmt {
	BinOp *condition;
	StmtList *then_stmts;
	StmtList *else_stmts;
} IfStmt;

IfStmt *
init_if_stmt ();

#endif//_IF_STMT_H_
