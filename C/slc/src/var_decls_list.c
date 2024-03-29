#include "../hdr/var_decl_list.h"

struct VarDecl *
init_vd () {

	struct VarDecl *vd = calloc(1, sizeof(struct VarDecl));
	vd->dimensions  = init_num_list();
	vd->identifiers = init_ident_list();
	return vd;
}

VarDeclListNode *
init_vdln () {

	VarDeclListNode *vdln;
	vdln       = calloc(1, sizeof(VarDeclListNode));
	vdln->data = init_vd();
	vdln->next = NULL;
	return vdln;
}

VarDeclList *
init_vdl () {

	VarDeclList *vdl = calloc(1, sizeof(VarDeclList));
	vdl->head        = NULL;
	return vdl;
}

// assume data already calloc'd
VarDeclListNode *
build_vdln (struct VarDecl *data) {

	VarDeclListNode *vdln;
	vdln       = calloc(1, sizeof(VarDeclListNode));
	vdln->data = data;
	vdln->next = NULL;
	return vdln;
}

void
next_vdln (VarDeclListNode **vdln) {

	(*vdln) = (*vdln)->next;
}

// assume new_node already calloc'd
void
push_vdln (VarDeclList **vdl,
					 VarDeclListNode *new_node) {
	
	VarDeclListNode *i = (*vdl)->head;
	if ( i == NULL ) {
		(*vdl)->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

// assume new_data already calloc'd
void
push_vdl_data (VarDeclList **vdl,
							 struct VarDecl *new_data) {
	
	VarDeclListNode *new_node = build_vdln(new_data);
	push_vdln(vdl, new_node);
}

void
print_vdl (VarDeclList *vdl) {

	int idx = 0;
  for (VarDeclListNode *i = vdl->head;
			 i != NULL;
			 i = i->next) {
		
		printf("dimens[%i]:", idx);
		print_num_list(i->data->dimensions);
		printf("idents[%i]:", idx);
		print_ident_list(i->data->identifiers);
  }
}

void
free_vdl (VarDeclList **vdl) {

	VarDeclListNode *cur = (*vdl)->head;
	VarDeclListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free_num_list(&prv->data->dimensions);
		free_ident_list(&prv->data->identifiers);
		free(prv);
	}
}
