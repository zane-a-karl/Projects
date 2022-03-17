#ifndef _FUNC_CALL_H_
#define _FUNC_CALL_H_

#include "../hdr/identifier_list.h"
#include "../hdr/result_list.h"

typedef struct FuncCall {
	Identifier *ident;
	ResultList *arg_exprs;
} FuncCall;

FuncCall *
init_func_call ();

#endif//_FUNC_CALL_H_
