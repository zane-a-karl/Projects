#include "../hdr/bin_op.h"


struct BinOp *
new_bin_op ()
{
	struct BinOp *bo = calloc(1, sizeof(struct BinOp));
	bo->lhs          = NULL;
	bo->op           = calloc(2, sizeof(char));
	bo->rhs          = NULL;
	return bo;
}
