#include "../hdr/bin_op.h"


struct BinOp *
new_bin_op ()
{
	struct BinOp *bo = calloc(1, sizeof(struct BinOp));
	bo->opa          = NULL;
	bo->op           = calloc(3, sizeof(char));
	bo->opb          = NULL;
	return bo;
}

struct BinOp *
deep_copy_bin_op (struct BinOp *src)
{
	struct BinOp *dst = new_bin_op();
	dst->opa = deep_copy_ast_node(src->opa);
	dst->op  = src->op;
	dst->opb = deep_copy_ast_node(src->opb);

	return dst;
}
