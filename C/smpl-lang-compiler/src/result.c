#include "../hdr/result.h"

void
set_result (Result *r,
						ResultType t,
						TokenNode *tn) {

	r->type = t;
	switch (t) {
	case CONSTANT:
		r->data = tn->tkn->val;
		break;
	case REGISTER:// This should be the same as VARIABLE
	case VARIABLE:
		r->data = lookup(tn->tkn->raw_tkn);
		break;
	default:
		perror("Infeasible result");
		exit(1);
		break;
	}
}
