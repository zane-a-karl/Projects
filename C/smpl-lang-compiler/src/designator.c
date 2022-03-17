#include "../hdr/designator.h"

Designator *
init_designator () {

	Designator *des = calloc(1, sizeof(Designator));
	des->ident      = init_ident();
	des->indices    = init_res_list();
	return des;
}
