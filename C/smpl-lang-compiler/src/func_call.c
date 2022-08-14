#include "../hdr/func_call.h"

struct FuncCall *
new_func_call ()
{
	struct FuncCall *fc = calloc(1, sizeof(struct FuncCall));
	fc->ident           = NULL;
	fc->args            = new_ast_node_list();
	
	return fc;
}

struct FuncCall *
deep_copy_func_call (struct FuncCall *src)
{
	struct FuncCall *dst = new_func_call();
	dst->ident = deep_copy_ast_node(src->ident);
	dst->args  = deep_copy_ast_node_list(src->args);

	return dst;
}
