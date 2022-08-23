#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "../hdr/ast.h"
#include "../hdr/interpreter_ctx.h"

#include <graphviz/cgraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct AstNode;

struct Number {
	int val;
};

struct Number *
new_number ();

struct Number *
deep_copy_number (struct Number *src);

void
create_num_agedge_set (char *label,
											 int len,
											 struct AstNode *n);

int
interpret_number (struct AstNode *n,
									struct InterpreterCtx *ictx);

#endif//_NUMBER_H_
