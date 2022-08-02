#include "../hdr/func_decl_list.h"

struct FuncDecl *
new_fd ()
{
	struct FuncDecl *fd = calloc(1, sizeof(struct FuncDecl));
	fd->is_void         = false;
	fd->name            = NULL;
	fd->params          = NULL;
	fd->body            = NULL;
	return fd;
}

struct FuncBody *
new_func_body ()
{
	struct FuncBody *fb   = calloc(1, sizeof(struct FuncBody));
	fb->local_vars = new_vdl();
	fb->stmts      = new_sl();
	return fb;
}

struct FuncDeclNode *
new_fdn (struct FuncDecl *fd)
{
	struct FuncDeclNode *fdn = calloc(1, sizeof(struct FuncDeclNode));
	fdn->data                = fd;
	fdn->next                = NULL;
	return fdn;
}

struct FuncDeclList *
new_fdl ()
{
	struct FuncDeclList *fdl = calloc(1, sizeof(struct FuncDeclList));
	fdl->head                = NULL;
	return fdl;
}

// assume new_node already calloc'd
void
push_fdn (struct FuncDeclList *fdl,
					struct FuncDeclNode *new_node)
{
	struct FuncDeclNode *i = fdl->head;
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
push_fd (struct FuncDeclList *fdl,
				 struct FuncDecl *new_data)
{
	push_fdn(fdl, new_fdn(new_data));
}

void
print_fdl (struct FuncDeclList *fdl)
{
	//	int idx = 0;
  for (struct FuncDeclNode *i = fdl->head;
			 i != NULL;
			 i = i->next) {

		/* printf("dimens[%i]:", idx); */
		/* print_num_list(i->dimens_list); */
		/* printf("idents[%i]:", idx); */
		/* print_ident_list(i->idents_list); */
  }
}

void
free_fdl (struct FuncDeclList **fdl)
{
	struct FuncDeclNode *cur = (*fdl)->head;
	struct FuncDeclNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur = cur->next;
		free_ident_list(&prv->data->params);
		free_vdl(&prv->data->body->local_vars);
		free_sl(&prv->data->body->stmts);
		free(prv);
	}
}
