#ifndef _BIN_OP_H_
#define _BIN_OP_H_

#include "../hdr/ast.h"

struct BinOp {
	struct AstNode *opa;
	char           *op;
	struct AstNode *opb;
};

struct BinOp *
new_bin_op ();

struct BinOp *
deep_copy_bin_op (struct BinOp *src);

#endif//_BIN_OP_H_
