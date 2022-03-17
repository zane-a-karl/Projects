#include "../hdr/assignment.h"

Assignment *
init_assignment () {

	Assignment *asn = calloc(1, sizeof(Assignment));
	asn->lhs        = init_designator();
	asn->rhs        = init_result();
	return asn;
}
