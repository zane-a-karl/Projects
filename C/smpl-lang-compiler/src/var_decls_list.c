#include "../hdr/var_decl_list.h"

struct VarDecl *
new_vd ()
{
	struct VarDecl *vd = calloc(1, sizeof(struct VarDecl));
	vd->dimensions     = NULL;
	vd->identifiers    = new_ident_list();

	return vd;
}

struct VarDeclNode *
new_vdn (struct VarDecl *vd)
{
	struct VarDeclNode *vdn = calloc(1, sizeof(struct VarDeclNode));
	vdn->data               = vd;
	vdn->next               = NULL;

	return vdn;
}

struct VarDeclList *
new_vdl ()
{
	struct VarDeclList *vdl = calloc(1, sizeof(struct VarDeclList));
	vdl->head               = NULL;

	return vdl;
}

// assume new_node already calloc'd
void
push_vdn (struct VarDeclList *vdl,
					struct VarDeclNode *new_node)
{	
	struct VarDeclNode *i = vdl->head;
	if ( i == NULL ) {
		vdl->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

// assume `data` already calloc'd
void
push_vd (struct VarDeclList *vdl,
				 struct VarDecl *data)
{	
	push_vdn(vdl, new_vdn(data));
}

void
print_vdl (struct VarDeclList *vdl)
{
	int idx = 0;
  for (struct VarDeclNode *i = vdl->head;
			 i != NULL;
			 i = i->next) {
		
		printf("dimens[%i]:", idx);
		print_num_list(i->data->dimensions);
		printf("idents[%i]:", idx);
		print_ident_list(i->data->identifiers);
  }
}

void
free_vdl (struct VarDeclList **vdl)
{
	struct VarDeclNode *cur = (*vdl)->head;
	struct VarDeclNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free_num_list(&prv->data->dimensions);
		free_ident_list(&prv->data->identifiers);
		free(prv);
	}
}
