#ifndef _DESIGNATOR_H_
#define _DESIGNATOR_H_

#include "../hdr/token.h"
#include "../hdr/result_list.h"

typedef struct Designator {
	char *name;
	ResultList *indices;
} Designator;

Designator *
init_designator ();

void
set_var_name (Designator **des,
							TokenNode **tn);

#endif//_DESIGNATOR_H_
