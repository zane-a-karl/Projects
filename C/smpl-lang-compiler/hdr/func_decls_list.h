#ifndef _FUNC_DECLS_LIST_H_
#define _FUNC_DECLS_LIST_H_

#include "../hdr/token.h"
#include "../hdr/str_list.h"
#include "../hdr/var_decls_list.h"
#include "../hdr/stmts_list.h"

#include <stdbool.h>
#include <stdlib.h>

#ifndef MAX_VAR_NAME_LEN
#define MAX_VAR_NAME_LEN 1<<4 //16
#endif//MAX_VAR_NAME_LEN

typedef struct FuncDeclsListNode {
	bool is_void;
	char *fn_name;
	StrList *param_idents;
	VarDeclsList *local_var_decls;
	StmtsList *stmts;
	struct FuncDeclsListNode *next;
} FuncDeclsListNode;

typedef struct FuncDeclsList {
	FuncDeclsListNode *head;
} FuncDeclsList;

FuncDeclsList *
init_func_decls_list ();

FuncDeclsListNode *
init_func_decls_list_node ();

void
set_fn_name (FuncDeclsListNode **fdln,
						 TokenNode **tn);

void
next_func_decls_list_node (FuncDeclsListNode **fdln);

void
push_func_decls_list_node (FuncDeclsList **fdl,
													 FuncDeclsListNode *new_node);

void
print_func_decls_list (FuncDeclsList *fdl);

void
free_func_decls_list (FuncDeclsList **fdl);

#endif//_FUNC_DECLS_LIST_H_
