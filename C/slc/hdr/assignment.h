#ifndef _ASSIGNMENT_H_
#define _ASSIGNMENT_H_

#include "../hdr/result_list.h"
#include "../hdr/designator.h"

typedef struct Assignment {
	Designator *lhs;
	union Result *rhs;
} Assignment;

Assignment *
init_assignment ();

#endif//_ASSIGNMENT_H_
