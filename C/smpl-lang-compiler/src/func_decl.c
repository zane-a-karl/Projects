#include "../hdr/func_decl.h"

struct FuncDecl *
new_fd ()
{
	struct FuncDecl *fd = calloc(1, sizeof(struct FuncDecl));
	fd->is_void         = false;
	fd->fn_ident        = NULL;
	fd->param_idents    = NULL;
	fd->body            = NULL;
	return fd;
}

struct FuncBody *
new_func_body ()
{
	struct FuncBody *fb = calloc(1, sizeof(struct FuncBody));
	fb->local_vars      = new_ast_node_list();
	fb->stmts           = NULL;

	return fb;
}

struct FuncBody *
deep_copy_func_body (struct FuncBody *src)
{
	struct FuncBody *dst = new_func_body();
	dst->local_vars = deep_copy_ast_node_list(src->local_vars);
	dst->stmts = deep_copy_ast_node_list(src->stmts);

	return dst;
}

struct FuncDecl *
deep_copy_func_decl (struct FuncDecl *src)
{
	struct FuncDecl *dst = new_fd();
	dst->is_void = src->is_void;
	dst->fn_ident = deep_copy_ast_node(src->fn_ident);
	dst->param_idents =
		deep_copy_ast_node_list(src->param_idents);
	dst->body = deep_copy_func_body(src->body);
	
	return dst;
}
