#include "../hdr/bin_op.h"

BinOp *
new_bin_op ()
{
	BinOp *bo = calloc(1, sizeof(BinOp));
	bo->lhs   = NULL;
	bo->op    = calloc(2, sizeof(char));
	bo->rhs   = NULL;
	return bo;
}
