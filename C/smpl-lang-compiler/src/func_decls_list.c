#include "../hdr/func_decls_list.h"

FuncDeclsList *
init_func_decls_list () {

	FuncDeclsList *fdl;
	fdl = (FuncDeclsList *)calloc(1, sizeof(FuncDeclsList));
	fdl->head = NULL;
	return fdl;
}

FuncDeclsListNode *
init_func_decls_list_node () {

	FuncDeclsListNode *fdln;
	fdln = (FuncDeclsListNode *)calloc(1,
																		sizeof(FuncDeclsListNode));
	fdln->is_void = false;
	fdln->fn_name = (char *)calloc(MAX_VAR_NAME_LEN,
																 sizeof(char));
	fdln->param_idents = init_str_list();
	fdln->local_var_decls = init_var_decls_list();
	fdln->stmts = init_stmts_list();
	fdln->next = NULL;
	return fdln;
}

void
set_fn_name (FuncDeclsListNode **fdln,
						 TokenNode **tn) {

	for (int i = 0; i < MAX_VAR_NAME_LEN; ++i) {
		(*fdln)->fn_name[i] = (*tn)->tkn->raw[i];
	}
}

void
next_func_decls_list_node (FuncDeclsListNode **fdln) {

	(*fdln) = (*fdln)->next;
}

// assume new_node already calloc'd
void
push_func_decls_list_node (FuncDeclsList **fdl,
													FuncDeclsListNode *new_node) {
	
	FuncDeclsListNode *i = (*fdl)->head;
	if ( i == NULL ) {
		(*fdl)->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

void
print_func_decls_list (FuncDeclsList *fdl) {

	int idx = 0;
  for (FuncDeclsListNode *i = fdl->head;
			 i != NULL;
			 i = i->next) {
		
		printf("dimens[%i]:", idx);
		print_int_list(i->dimens_list);
		printf("idents[%i]:", idx);
		print_str_list(i->idents_list);
  }
}

void
free_func_decls_list (FuncDeclsList **fdl) {

	FuncDeclsListNode *cur = (*fdl)->head;
	FuncDeclsListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free_int_list(prv->dimens_list);
		free_str_list(prv->idents_list);
		free(prv);
	}
}

