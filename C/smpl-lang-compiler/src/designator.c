#include "../hdr/designator.h"

Designator *
init_designator () {

	Designator *des = calloc(1, sizeof(Designator));
	des->name       = calloc(MAX_VAR_NAME_LEN, sizeof(char));
	des->indices    = init_result_list();
	return des;
}

void
set_var_name (Designator **des,
							TokenNode **tn) {

	for (int i = 0; i < MAX_VAR_NAME_LEN; ++i) {
		(*des)->name[i] = (*tn)->tkn->raw[i];
	}
}
