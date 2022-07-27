#include "../hdr/func_decl_list.h"

FuncDecl *
new_fd ()
{
	FuncDecl *fd = calloc(1, sizeof(FuncDecl));
	fd->is_void         = false;
	fd->name            = NULL;
	fd->params          = NULL;
	fd->body            = NULL;
	return fd;
}

FuncBody *
new_func_body ()
{
	FuncBody *fb   = calloc(1, sizeof(FuncBody));
	fb->local_vars = new_vdl();
	fb->stmts      = new_sl();
	return fb;
}

FuncDeclNode *
new_fdn (FuncDecl *fd)
{
	FuncDeclNode *fdn = calloc(1, sizeof(FuncDeclNode));
	fdn->data         = fd;
	fdn->next         = NULL;
	return fdn;
}

FuncDeclList *
new_fdl ()
{
	FuncDeclList *fdl = calloc(1, sizeof(FuncDeclList));
	fdl->head         = NULL;
	return fdl;
}

// assume new_node already calloc'd
void
push_fdn (FuncDeclList *fdl,
					FuncDeclNode *new_node)
{
	FuncDeclNode *i = fdl->head;
	if ( i == NULL ) {
		fdl->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

// assume `new_data` already calloc'd
void
push_fd (FuncDeclList *fdl,
				 FuncDecl *new_data)
{
	push_fdn(fdl, new_fdn(new_data));
}

void
print_fdl (FuncDeclList *fdl)
{
	//	int idx = 0;
  for (FuncDeclNode *i = fdl->head;
			 i != NULL;
			 i = i->next) {

		/* printf("dimens[%i]:", idx); */
		/* print_num_list(i->dimens_list); */
		/* printf("idents[%i]:", idx); */
		/* print_ident_list(i->idents_list); */
  }
}

void
free_fdl (FuncDeclList **fdl)
{
	FuncDeclNode *cur = (*fdl)->head;
	FuncDeclNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur = cur->next;
		free_ident_list(&prv->data->params);
		free_vdl(&prv->data->body->local_vars);
		free_sl(&prv->data->body->stmts);
		free(prv);
	}
}
