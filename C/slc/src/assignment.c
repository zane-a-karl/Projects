#include "../hdr/assignment.h"

Assignment *
init_assignment () {

	Assignment *a = calloc(1, sizeof(Assignment));
	a->lhs        = init_designator();
	a->rhs        = init_res();
	return a;
}
