#include "../hdr/computation.h"

Computation *
init_computation () {

	Computation *c = calloc(1, sizeof(Computation));
	c->var_decls   = init_vdl();
	c->func_decls  = init_fdl();
	c->stmts       = init_sl();
	return c;
}
