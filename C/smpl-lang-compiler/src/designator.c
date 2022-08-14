#include "../hdr/designator.h"

struct Designator *
new_designator ()
{
	struct Designator *d = calloc(1, sizeof(struct Designator));
	d->ident             = NULL;
	d->indices           = new_ast_node_list();

	return d;
}

struct Designator *
deep_copy_designator (struct Designator *src)
{
	struct Designator *dst = new_designator();
	dst->ident = deep_copy_ast_node(src->ident);
	dst->indices = deep_copy_ast_node_list(src->indices);

	return dst;
}
