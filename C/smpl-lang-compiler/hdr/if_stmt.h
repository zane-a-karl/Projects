#ifndef _IF_STMT_H_
#define _IF_STMT_H_

#include "../hdr/ast.h"

struct IfStmt {
	struct AstNode     *condition;
	struct AstNodeList *then_stmts;
	struct AstNodeList *else_stmts;
};

struct IfStmt *
new_if_stmt ();

struct IfStmt *
deep_copy_if_stmt (struct IfStmt *src);

#endif//_IF_STMT_H_
