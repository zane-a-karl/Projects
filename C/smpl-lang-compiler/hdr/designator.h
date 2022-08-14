#ifndef _DESIGNATOR_H_
#define _DESIGNATOR_H_

#include "../hdr/ast.h"

struct Designator {
	struct AstNode     *ident;
	struct AstNodeList *indices;
};

struct Designator *
new_designator ();

struct Designator *
deep_copy_designator (struct Designator *src);

#endif//_DESIGNATOR_H_
