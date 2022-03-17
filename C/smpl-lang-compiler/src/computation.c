#include "../hdr/computation.h"

Computation *
init_computation () {

	Computation *c = calloc(1, sizeof(Computation));
	c->var_decls   = init_var_decl_list();
	c->func_decls  = init_func_decl_list();
	c->stmts       = init_stmt_list();
	return c;
}
