#ifndef _FUNC_CALL_H_
#define _FUNC_CALL_H_

#include "../hdr/identifier_list.h"
#include "../hdr/result_list.h"

struct FuncCall {
	struct Ident      *name;
	struct ResultList *args;
};

struct FuncCall *
new_func_call ();

#endif//_FUNC_CALL_H_
