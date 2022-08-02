#ifndef _WHILE_STMT_H_
#define _WHILE_STMT_H_

#include "../hdr/stmt_list.h"
//#include "../hdr/bin_op.h"
#include "../hdr/result_list.h"


struct WhileStmt {
	struct BinOp *condition;
	struct StmtList *do_stmts;
};

struct WhileStmt *
new_while_stmt ();

#endif//_WHILE_STMT_H_
