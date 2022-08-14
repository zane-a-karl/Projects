#ifndef _ASSIGNMENT_H_
#define _ASSIGNMENT_H_

#include "../hdr/ast.h"

struct Assignment {
	struct AstNode *lhs;
	struct AstNode *rhs;
};

struct Assignment *
new_assignment ();

struct Assignment *
deep_copy_assignment (struct Assignment *src);

#endif//_ASSIGNMENT_H_
