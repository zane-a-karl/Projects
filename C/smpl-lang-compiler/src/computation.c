#include "../hdr/computation.h"

struct Computation *
new_computation ()
{
	struct Computation *c;
	c             = calloc(1, sizeof(struct Computation));
	c->var_decls  = new_ast_node_list();
	c->func_decls = new_ast_node_list();
	c->stmts      = NULL;

	return c;
}

/* struct AstNodeList * */
/* computation_edge_set (struct Computation *c) */
/* { */

/* } */

struct Computation *
deep_copy_computation (struct Computation *src)
{
	struct Computation *dst = new_computation();
	dst->var_decls = deep_copy_ast_node_list(src->var_decls);
	dst->func_decls = deep_copy_ast_node_list(src->func_decls);
	dst->stmts = deep_copy_ast_node_list(src->stmts);

	return dst;
}
