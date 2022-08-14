#include "../hdr/var_decl.h"

struct VarDecl *
new_vd ()
{
	struct VarDecl *vd = calloc(1, sizeof(struct VarDecl));
	vd->dims           = NULL;
	vd->ident          = NULL;

	return vd;
}

struct VarDecl *
deep_copy_var_decl (struct VarDecl *src)
{
	struct VarDecl *dst = new_vd();
	dst->ident = deep_copy_ast_node(src->ident);
	dst->dims = deep_copy_ast_node_list(src->dims);

	return dst;
}
