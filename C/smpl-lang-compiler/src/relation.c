#include "../hdr/relation.h"

Relation *
init_relation () {

	Relation *rel = calloc(1, sizeof(Relation));
	rel->lhs      = init_result_list();
	rel->rel_op   = calloc(2, sizeof(char));
	rel->rhs      = init_result_list();
	return rel;
}
