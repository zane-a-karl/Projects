#include "../hdr/assignment.h"

struct Assignment *
new_assignment ()
{
	struct Assignment *a = calloc(1, sizeof(struct Assignment));
	a->lhs               = NULL;
	a->rhs               = NULL;
	return a;
}
