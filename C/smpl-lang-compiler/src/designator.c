#include "../hdr/designator.h"

Designator *
init_designator () {

	Designator *des = calloc(1, sizeof(Designator));
	des->ident      = init_identifier();
	des->indices    = init_result_list();
	return des;
}
