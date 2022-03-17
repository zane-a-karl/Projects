#include "../hdr/func_call.h"

FuncCall *
init_func_call () {

	FuncCall *fc  = calloc(1, sizeof(FuncCall));
	fc->ident     = init_identifier();
	fc->arg_exprs = init_result_list();
	return fc;
}
