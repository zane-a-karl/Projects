#ifndef _WHILE_STMT_H_
#define _WHILE_STMT_H_

#include "../hdr/ast.h"

struct WhileStmt {
	struct AstNode     *condition;
	struct AstNodeList *do_stmts;
};

struct WhileStmt *
new_while_stmt ();

struct WhileStmt *
deep_copy_while_stmt (struct WhileStmt *src);

#endif//_WHILE_STMT_H_
