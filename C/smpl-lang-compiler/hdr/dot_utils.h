#ifndef _DOT_UTILS_H_
#define _DOT_UTILS_H_

#include "../hdr/ast.h"

#include <stdio.h>


void
gen_dot_graph (struct Ast *ast,
							 FILE *fout);

#endif//_DOT_UTILS_H_
