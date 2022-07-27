#include "../hdr/assignment.h"

Assignment *
new_assignment ()
{
	Assignment *a = calloc(1, sizeof(Assignment));
	a->lhs        = NULL;
	a->rhs        = NULL;
	return a;
}
