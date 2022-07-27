#include "../hdr/computation.h"

Computation *
new_computation ()
{
	Computation *c = calloc(1, sizeof(Computation));
	c->var_decls   = new_vdl();
	c->func_decls  = new_fdl();
	c->stmts       = NULL;

	return c;
}
