#include "../hdr/func_call.h"

struct FuncCall *
new_func_call ()
{
	struct FuncCall *fc = calloc(1, sizeof(struct FuncCall));
	fc->name     = NULL;
	fc->args     = new_res_list();
	return fc;
}
