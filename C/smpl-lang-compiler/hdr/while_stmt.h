#ifndef _WHILE_STMT_H_
#define _WHILE_STMT_H_

#include "../hdr/stmt_list.h"
#include "../hdr/bin_op.h"

typedef struct WhileStmt {
	BinOp *condition;
	StmtList *do_stmts;
} WhileStmt;

WhileStmt *
init_while_stmt ();

#endif//_WHILE_STMT_H_
