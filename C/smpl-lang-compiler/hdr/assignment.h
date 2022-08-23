#ifndef _ASSIGNMENT_H_
#define _ASSIGNMENT_H_

#include "../hdr/ast.h"
#include "../hdr/interpreter_ctx.h"

struct Assignment {
	struct AstNode *lhs;
	struct AstNode *rhs;
};

struct Assignment *
new_assignment ();

struct Assignment *
deep_copy_assignment (struct Assignment *src);

void
create_assignment_agedge_set (char *label,
															int len,
															struct AstNode *n);

int
interpret_assignment (struct AstNode *n,
											struct InterpreterCtx *ictx);

#endif//_ASSIGNMENT_H_
