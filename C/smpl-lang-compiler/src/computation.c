#include "../hdr/computation.h"

struct Computation *
new_computation ()
{
	struct Computation *c = calloc(1, sizeof(struct Computation));
	c->var_decls          = new_vdl();
	c->func_decls         = new_fdl();
	c->stmts              = NULL;

	return c;
}
