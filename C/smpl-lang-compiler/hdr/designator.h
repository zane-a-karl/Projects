#ifndef _DESIGNATOR_H_
#define _DESIGNATOR_H_

#include "../hdr/token.h"
#include "../hdr/identifier_list.h"
#include "../hdr/result_list.h"

struct Designator {
	struct Ident      *ident;
	struct ResultList *indices;
};

struct Designator *
new_designator ();

#endif//_DESIGNATOR_H_
