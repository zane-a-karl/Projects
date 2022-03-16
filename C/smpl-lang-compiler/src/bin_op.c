#include "../hdr/bin_op.h"

BinOp *
init_bin_op () {

	BinOp *bo = calloc(1, sizeof(BinOp));
	return bo;
}

Operand *
init_operand () {

	Operand *opnd = calloc(1, sizeof(Operand));
	return opnd;
}
