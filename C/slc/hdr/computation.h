#ifndef _COMPUTATION_H_
#define _COMPUTATION_H_

#include "../hdr/stmt_list.h"
#include "../hdr/var_decl_list.h"
#include "../hdr/func_decl_list.h"

typedef struct Computation {
	VarDeclList *var_decls;
	FuncDeclList *func_decls;
	StmtList *stmts;
} Computation;

Computation *
init_computation ();

#endif//_COMPUTATION_H_
