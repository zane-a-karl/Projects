#ifndef _DESIGNATOR_H_
#define _DESIGNATOR_H_

// Do this to silence circular dependency
typedef struct Designator Designator;

#include "../hdr/token.h"
#include "../hdr/identifier_list.h"
#include "../hdr/result_list.h"

struct Designator {
	Ident *ident;
	struct ResultList *indices;
};

Designator *
init_designator ();

#endif//_DESIGNATOR_H_
