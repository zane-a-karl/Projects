#include "../hdr/designator.h"

struct Designator *
new_designator ()
{
	struct Designator *des = calloc(1, sizeof(struct Designator));
	des->ident             = NULL;
	des->indices           = new_res_list();

	return des;
}
