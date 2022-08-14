#include "../hdr/assignment.h"

struct Assignment *
new_assignment ()
{
	struct Assignment *a = calloc(1, sizeof(struct Assignment));
	a->lhs               = NULL;
	a->rhs               = NULL;

	return a;
}

struct Assignment *
deep_copy_assignment (struct Assignment *src)
{
	struct Assignment *dst = new_assignment();
	dst->lhs = deep_copy_ast_node(src->lhs);
	dst->rhs = deep_copy_ast_node(src->rhs);

	return dst;
}
