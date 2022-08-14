#ifndef _RETURN_STMT_H_
#define _RETURN_STMT_H_

#include "../hdr/ast.h"

struct ReturnStmt {
	struct AstNode *ret_val;
};

struct ReturnStmt *
new_return_stmt ();

struct ReturnStmt *
deep_copy_return_stmt (struct ReturnStmt *src);

#endif//_RETURN_STMT_H_
