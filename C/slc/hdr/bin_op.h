#ifndef _BIN_OP_H_
#define _BIN_OP_H_

// Do this to silence circular dependency
typedef struct BinaryOp BinOp;

#include "../hdr/result_list.h"

struct BinaryOp {
	union Result *lhs;
	char *op;
	union Result *rhs;
};

BinOp *
init_bin_op ();

#endif//_BIN_OP_H_
