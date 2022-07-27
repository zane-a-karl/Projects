#include "../hdr/designator.h"

Designator *
new_designator ()
{
	Designator *des = calloc(1, sizeof(Designator));
	des->ident      = NULL;
	des->indices    = new_res_list();
	return des;
}
