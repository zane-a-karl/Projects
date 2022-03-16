#ifndef _COMPUTATION_H_
#define _COMPUTATION_H_

#include "../hdr/var_decls_list.h"
#include "../hdr/func_decls_list.h"

typedef struct Computation {
	VarDeclsList *vdl;
	FuncDeclsList *fdl;
	StmtsList *sl;
} Computation;

Computation *
init_computation ();

#endif//_COMPUTATION_H_
