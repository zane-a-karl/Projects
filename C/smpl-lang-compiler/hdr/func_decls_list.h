#ifndef _FUNC_DECLS_LIST_H_
#define _FUNC_DECLS_LIST_H_

#include "../hdr/int_list.h"
#include "../hdr/str_list.h"

#include <stdlib.h>

typedef struct FuncDeclsListNode {
	IntList *dimens_list;
	StrList *idents_list;
} FuncDeclsListNode;

typedef struct FuncDeclsList {
	FuncDeclsListNode *head;
} FuncDeclsList;

FuncDeclsList *
init_func_decls_list ();

FuncDeclsListNode *
init_func_decls_list_node ();

void
next_func_decls_list_node (FuncDeclsListNode **fdln);

void
clear_func_decls_list_node (FuncDeclsListNode **fdln);

void
push_func_decls_list_node (FuncDeclsList **fdl,
													FuncDeclsListNode *new_node);

void
print_func_decls_list (FuncDeclsList *fdl);

void
free_func_decls_list (FuncDeclsList **fdl);

#endif//_FUNC_DECLS_LIST_H_
