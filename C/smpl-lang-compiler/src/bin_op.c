#include "../hdr/bin_op.h"

BinOp *
init_bin_op () {

	BinOp *bo = calloc(1, sizeof(BinOp));
	bo->lhs   = init_res();
	bo->op    = calloc(2, sizeof(char));
	bo->rhs   = init_res();
	return bo;
}
