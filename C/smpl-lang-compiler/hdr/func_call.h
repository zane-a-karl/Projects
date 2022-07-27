#ifndef _FUNC_CALL_H_
#define _FUNC_CALL_H_

#include "../hdr/identifier_list.h"
#include "../hdr/result_list.h"

typedef struct FuncCall {
	Ident *name;
	struct ResultList *args;
} FuncCall;

FuncCall *
new_func_call ();

#endif//_FUNC_CALL_H_
