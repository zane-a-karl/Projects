#ifndef _VAR_DECLS_LIST_H_
#define _VAR_DECLS_LIST_H_

#include "../hdr/int_list.h"
#include "../hdr/str_list.h"

#include <stdlib.h>

typedef struct VarDeclsListNode {
	IntList *dimens_list;
	StrList *idents_list;
	struct VarDeclsListNode *next;
} VarDeclsListNode;

typedef struct VarDeclsList {
	VarDeclsListNode *head;
} VarDeclsList;

VarDeclsList *
init_var_decls_list ();

VarDeclsListNode *
init_var_decls_list_node ();

void
next_var_decls_list_node (VarDeclsListNode **vdln);

void
push_var_decls_list_node (VarDeclsList **vdl,
													VarDeclsListNode *new_node);

void
print_var_decls_list (VarDeclsList *vdl);

void
free_var_decls_list (VarDeclsList **vdl);

#endif//_VAR_DECLS_LIST_H_
