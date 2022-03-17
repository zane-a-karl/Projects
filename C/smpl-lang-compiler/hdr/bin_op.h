#ifndef _BIN_OP_H_
#define _BIN_OP_H_

#include "../hdr/result_list.h"

typedef struct BinaryOperation {
	Result opa;
	char operator;
	Result opb;
} BinOp;

BinOp *
init_bin_op ();

#endif//_BIN_OP_H_
