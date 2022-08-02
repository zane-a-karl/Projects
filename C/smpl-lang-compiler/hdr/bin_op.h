#ifndef _BIN_OP_H_
#define _BIN_OP_H_

#include "../hdr/result_list.h"

struct BinOp {
	struct Result *lhs;
	char          *op;
	struct Result *rhs;
};

struct BinOp *
new_bin_op ();

#endif//_BIN_OP_H_
