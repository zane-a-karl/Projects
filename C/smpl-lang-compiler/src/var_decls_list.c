#include "../hdr/var_decl_list.h"

struct VarDecl *
new_vd ()
{
	struct VarDecl *vd = calloc(1, sizeof(struct VarDecl));
	vd->dimensions     = NULL;
	vd->identifiers    = new_ident_list();
	return vd;
}

VarDeclNode *
new_vdn (VarDecl *vd)
{
	VarDeclNode *vdn = calloc(1, sizeof(VarDeclNode));
	vdn->data        = vd;
	vdn->next        = NULL;
	return vdn;
}

VarDeclList *
new_vdl ()
{
	VarDeclList *vdl = calloc(1, sizeof(VarDeclList));
	vdl->head        = NULL;
	return vdl;
}

// assume new_node already calloc'd
void
push_vdn (VarDeclList *vdl,
					VarDeclNode *new_node)
{	
	VarDeclNode *i = vdl->head;
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
push_vd (VarDeclList *vdl,
				 struct VarDecl *data)
{	
	push_vdn(vdl, new_vdn(data));
}

void
print_vdl (VarDeclList *vdl)
{
	int idx = 0;
  for (VarDeclNode *i = vdl->head;
			 i != NULL;
			 i = i->next) {
		
		printf("dimens[%i]:", idx);
		print_num_list(i->data->dimensions);
		printf("idents[%i]:", idx);
		print_ident_list(i->data->identifiers);
  }
}

void
free_vdl (VarDeclList **vdl)
{
	VarDeclNode *cur = (*vdl)->head;
	VarDeclNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free_num_list(&prv->data->dimensions);
		free_ident_list(&prv->data->identifiers);
		free(prv);
	}
}
