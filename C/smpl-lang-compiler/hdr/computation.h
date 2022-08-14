#ifndef _COMPUTATION_H_
#define _COMPUTATION_H_

#include "../hdr/ast.h"

struct Computation {
	struct AstNodeList *var_decls;
	struct AstNodeList *func_decls;
	struct AstNodeList *stmts;
};

struct Computation *
new_computation ();

/* struct AstNodeList * */
/* computation_edge_set (struct Computation *c); */

struct Computation *
deep_copy_computation (struct Computation *src);

#endif//_COMPUTATION_H_
