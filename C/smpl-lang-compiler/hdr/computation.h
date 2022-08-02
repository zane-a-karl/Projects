#ifndef _COMPUTATION_H_
#define _COMPUTATION_H_

#include "../hdr/stmt_list.h"
#include "../hdr/var_decl_list.h"
#include "../hdr/func_decl_list.h"

struct Computation {
	struct VarDeclList *var_decls;
	struct FuncDeclList *func_decls;
	struct StmtList *stmts;
};

struct Computation *
new_computation ();

#endif//_COMPUTATION_H_
