#ifndef _FUNC_CALL_H_
#define _FUNC_CALL_H_

#include "../hdr/ast.h"

struct FuncCall {
	struct AstNode     *ident;
	struct AstNodeList *args;
};

struct FuncCall *
new_func_call ();

struct FuncCall *
deep_copy_func_call (struct FuncCall *src);

#endif//_FUNC_CALL_H_
