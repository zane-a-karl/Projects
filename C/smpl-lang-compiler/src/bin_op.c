#include "../hdr/bin_op.h"

BinOp *
init_bin_op () {

	BinOp *bo    = calloc(1, sizeof(BinOp));
	bo->opa      = init_result();
	bo->operator = '\0';
	bo->opb      = init_result();
	return bo;
}
