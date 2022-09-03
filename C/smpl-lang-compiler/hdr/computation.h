#ifndef _COMPUTATION_H_
#define _COMPUTATION_H_

#include "../hdr/ast.h"
#include "../hdr/interpreter_ctx.h"

struct AstNode;

struct Computation {
	struct AstNodeList *var_decls;
	struct AstNodeList *func_decls;
	struct AstNodeList *stmts;
};

struct Computation *
new_computation ();

struct Computation *
deep_copy_computation (struct Computation *src);

void
create_computation_agedge_set (char *label,
															 int len,
															 struct AstNode *n);

int
interpret_computation (struct AstNode *n,
											 struct InterpreterCtx *ictx);

void
compile_computation (struct AstNode *n,
										 struct CompilerCtx *cctx);

#endif//_COMPUTATION_H_
