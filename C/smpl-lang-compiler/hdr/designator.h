#ifndef _DESIGNATOR_H_
#define _DESIGNATOR_H_

#include "../hdr/token.h"
#include "../hdr/identifier_list.h"
#include "../hdr/result_list.h"

typedef struct Designator {
	Identifier *ident;
	ResultList *indices;
} Designator;

Designator *
init_designator ();

#endif//_DESIGNATOR_H_
