#ifndef _VAR_DECL_LIST_H_
#define _VAR_DECL_LIST_H_

typedef struct VarDecl VarDecl;

#include "../hdr/number_list.h"
#include "../hdr/identifier_list.h"

#include <stdlib.h>

struct VarDecl {
	NumList *dimensions;
	IdentList *identifiers;
};

typedef struct VarDeclNode {
	VarDecl *data;
	struct VarDeclNode *next;
} VarDeclNode;

typedef struct VarDeclList {
	VarDeclNode *head;
} VarDeclList;

VarDecl *
new_vd ();

VarDeclNode *
new_vdn (VarDecl *vd);

VarDeclList *
new_vdl ();

void
push_vdn (VarDeclList *vdl,
					VarDeclNode *new_node);

void
push_vd (VarDeclList *vdl,
				 VarDecl *data);

void
print_vdl (VarDeclList *vdl);

void
free_vdl (VarDeclList **vdl);

#endif//_VAR_DECL_LIST_H_
