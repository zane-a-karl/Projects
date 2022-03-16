#ifndef _RELATION_H_
#define _RELATION_H_

#include "../hdr/result_list.h"

typedef struct Relation {
	ResultList *lhs;
	char *rel_op;
	ResultList *rhs;
} Relation;

Relation *
init_relation ();

#endif//_RELATION_H_
