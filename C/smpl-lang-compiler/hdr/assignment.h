#ifndef _ASSIGNMENT_H_
#define _ASSIGNMENT_H_

#include "../hdr/result_list.h"
#include "../hdr/designator.h"

typedef struct Assignment {
	Designator *lhs;
	Result *rhs
} Assignment;

#endif//_ASSIGNMENT_H_
