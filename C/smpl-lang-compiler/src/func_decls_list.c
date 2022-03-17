#include "../hdr/func_decl_list.h"

FuncDecl *
init_fd () {

	FuncDecl *fd   = calloc(1, sizeof(FuncDecl));
	fd->ident      = init_identifier();
	fd->params     = init_identifier_list();
	fd->local_vars = init_var_decl_list();
	fd->stmts      = init_stmt_list();
	fd->is_void    = false;
	return fd;
}

FuncBody *
init_func_body () {

	FuncBody *fb   = calloc(1, sizeof(FuncBody));
	fb->local_vars = init_vdl();
	fb->stmts      = init_stmt_list();
	return fb;
}

FuncDeclListNode *
init_fdln () {

	FuncDeclListNode *fdln;
	fdln       = calloc(1, sizeof(FuncDeclListNode));
	fdln->data = init_fd();
	fdln->next = NULL;
	return fdln;
}

FuncDeclList *
init_fdl () {

	FuncDeclList *fdl = calloc(1, sizeof(FuncDeclList));
	fdl->head         = NULL;
	return fdl;
}

// assume data already calloc'd
FuncDeclListNode *
build_fdln (FuncDecl *data) {

	FuncDeclListNode *fdln;
	fdln       = calloc(1, sizeof(FuncDeclListNode));
	fdln->data = data;
	fdln->next = NULL;
	return fdln;
}

void
next_fdln (FuncDeclListNode **fdln) {

	(*fdln) = (*fdln)->next;
}

// assume new_node already calloc'd
void
push_fdln (FuncDeclList **fdl,
					 FuncDeclListNode *new_node) {
	
	FuncDeclListNode *i = (*fdl)->head;
	if ( i == NULL ) {
		(*fdl)->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

// assume new_data already calloc'd
void
push_fdl_data (FuncDeclList **fdl,
							 FuncDecl *new_data) {
	
	FuncDeclListNode *new_node = build_fdln(new_data);
	push_fdln(fdl, new_node);
}

void
print_fdl (FuncDeclList *fdl) {

	int idx = 0;
  for (FuncDeclListNode *i = fdl->head;
			 i != NULL;
			 i = i->next) {
		
		printf("dimens[%i]:", idx);
		print_int_list(i->dimens_list);
		printf("idents[%i]:", idx);
		print_str_list(i->idents_list);
  }
}

void
free_fdl (FuncDeclList **fdl) {

	FuncDeclListNode *cur = (*fdl)->head;
	FuncDeclListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free_int_list(prv->dimens_list);
		free_str_list(prv->idents_list);
		free(prv);
	}
}

