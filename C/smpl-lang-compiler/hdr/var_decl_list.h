#ifndef _VAR_DECL_LIST_H_
#define _VAR_DECL_LIST_H_

#include "../hdr/number_list.h"
#include "../hdr/identifier_list.h"

#include <stdlib.h>

typedef struct VarDecl {
	NumList *dimensions;
	IdentList *identifiers;
} Var_Decl;

typedef struct VarDeclListNode {
	VarDecl *data;
	struct VarDeclListNode *next;
} VarDeclListNode;

typedef struct VarDeclList {
	VarDeclListNode *head;
} VarDeclList;

VarDecl *
init_vd ();

VarDeclListNode *
init_vdln ();

VarDeclList *
init_vdl ();

VarDeclListNode *
build_vdln (VarDecl *vd);

void
next_vdln (VarDeclListNode **vdln);

void
push_vdln (VarDeclList **vdl,
					 VarDeclListNode *new_node);

void
push_vdl_data (VarDeclList **vdl,
							 VarDecl *new_data);

void
print_vdl (VarDeclList *vdl);

void
free_vdl (VarDeclList **vdl);

#endif//_VAR_DECL_LIST_H_
