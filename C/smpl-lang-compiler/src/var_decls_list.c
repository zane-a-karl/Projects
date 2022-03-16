#include "../hdr/var_decls_list.h"

VarDeclsList *
init_var_decls_list () {

	VarDeclsList *vdl;
	vdl = (VarDeclsList *)calloc(1, sizeof(VarDeclsList));
	vdl->head = NULL;
	return vdl;
}

VarDeclsListNode *
init_var_decls_list_node () {

	VarDeclsListNode *vdln;
	vdln = (VarDeclsListNode *)calloc(1,
																		sizeof(VarDeclsListNode));
	vdln->dimens_list = init_int_list();
	vdln->idents_list = init_str_list();
	vdln->next = NULL;
	return vdln;
}

void
next_var_decls_list_node (VarDeclsListNode **vdln) {

	(*vdln) = (*vdln)->next;
}

// assume new_node already calloc'd
void
push_var_decls_list_node (VarDeclsList **vdl,
													VarDeclsListNode *new_node) {
	
	VarDeclsListNode *i = (*vdl)->head;
	if ( i == NULL ) {
		(*vdl)->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

void
print_var_decls_list (VarDeclsList *vdl) {

	int idx = 0;
  for (VarDeclsListNode *i = vdl->head;
			 i != NULL;
			 i = i->next) {
		
		printf("dimens[%i]:", idx);
		print_int_list(i->dimens_list);
		printf("idents[%i]:", idx);
		print_str_list(i->idents_list);
  }
}

void
free_var_decls_list (VarDeclsList **vdl) {

	VarDeclsListNode *cur = (*vdl)->head;
	VarDeclsListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free_int_list(prv->dimens_list);
		free_str_list(prv->idents_list);
		free(prv);
	}
}

