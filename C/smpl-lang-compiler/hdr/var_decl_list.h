#ifndef _VAR_DECL_LIST_H_
#define _VAR_DECL_LIST_H_

#include "../hdr/number_list.h"
#include "../hdr/identifier_list.h"

#include <stdlib.h>

struct VarDecl {
	struct NumberList *dimensions;
	struct IdentList *identifiers;
};

struct VarDeclNode {
	struct VarDecl *data;
	struct VarDeclNode *next;
};

struct VarDeclList {
	struct VarDeclNode *head;
};

struct VarDecl *
new_vd ();

struct VarDeclNode *
new_vdn (struct VarDecl *vd);

struct VarDeclList *
new_vdl ();

void
push_vdn (struct VarDeclList *vdl,
					struct VarDeclNode *new_node);

void
push_vd (struct VarDeclList *vdl,
				 struct VarDecl *data);

void
print_vdl (struct VarDeclList *vdl);

void
free_vdl (struct VarDeclList **vdl);

#endif//_VAR_DECL_LIST_H_
