#include "../hdr/func_call.h"

FuncCall *
new_func_call ()
{
	FuncCall *fc = calloc(1, sizeof(FuncCall));
	fc->name     = NULL;
	fc->args     = new_res_list();
	return fc;
}
