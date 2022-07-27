#ifndef _BIN_OP_H_
#define _BIN_OP_H_

// Do this to silence circular dependency
typedef struct BinaryOp BinOp;

#include "../hdr/result_list.h"

struct BinaryOp {
	struct Result *lhs;
	char *op;
	struct Result *rhs;
};

BinOp *
new_bin_op ();

#endif//_BIN_OP_H_
