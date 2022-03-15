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
	fdln->dimens_list = init_int_list();
	fdln->idents_list = init_str_list();
	return fdln;
}

void
next_func_decls_list_node (FuncDeclsListNode **fdln) {

	(*vldn) = (*vldn)->next;
}

void
clear_func_decls_list_node (FuncDeclsListNode *fdln) {

	clear_int_list_node(fdln->dimens_list);
	clear_str_list_node(fdln->idents_list);
	fdln->next = NULL;
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
  for (FuncDeclsListNode *i = sl->head;
			 i != NULL;
			 i = i->next) {
		
		printf("dimens[%i]:", idx);
		print_int_list(i->dimens_list);
		printf("idents[%i]:", idx);
		print_str_list(i->idents_list);
  }
}

void
free_func_decls_list (FuncDeclsList *fdl) {

	FuncDeclsListNode *cur = fdl->head;
	FuncDeclsListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free_int_list(prv->dimens_list);
		free_str_list(prv->idents_list);
		free(prv);
	}
}

