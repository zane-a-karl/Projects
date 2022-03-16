#include "../hdr/computation.h"

Computation *
init_computation () {

	Computation *c = calloc(1, sizeof(Computation));
	c->vdl = init_var_decls_list();
	c->fdl = init_func_decls_list();
	c->sl  = init_stmts_list();
	return c;
}
