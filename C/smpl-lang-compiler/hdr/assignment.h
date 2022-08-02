#ifndef _ASSIGNMENT_H_
#define _ASSIGNMENT_H_

#include "../hdr/result_list.h"
#include "../hdr/designator.h"

struct Assignment {
	struct Designator *lhs;
	struct Result *rhs;
};

struct Assignment *
new_assignment ();

#endif//_ASSIGNMENT_H_
