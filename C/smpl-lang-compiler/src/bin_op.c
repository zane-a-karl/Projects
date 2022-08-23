#include "../hdr/bin_op.h"

struct BinOp *
new_bin_op ()
{
	struct BinOp *bo = calloc(1, sizeof(struct BinOp));
	bo->opa          = NULL;
	bo->op           = calloc(3, sizeof(char));
	bo->opb          = NULL;

	return bo;
}

struct BinOp *
deep_copy_bin_op (struct BinOp *src)
{
	struct BinOp *dst = new_bin_op();
	dst->opa = deep_copy_ast_node(src->opa);
	dst->op  = src->op;
	dst->opb = deep_copy_ast_node(src->opb);

	return dst;
}

void
create_bin_op_agedge_set (char *label,
													int len,
													struct AstNode *n)
{
	Agedge_t *edge;
	char *tmp = calloc(len, sizeof(char));
	snprintf(tmp, len, "%s", n->bin_op->op);
	agset(n->self, "label", tmp);
	edge = agedge(n->graph, n->self, n->bin_op->opa->self, "OP_A", TRUE);
	agset(edge, "label", "OP_A");
	create_agedge_set(n->bin_op->opa);
	edge = agedge(n->graph, n->self, n->bin_op->opb->self, "OP_B", TRUE);
	agset(edge, "label", "OP_B");
	create_agedge_set(n->bin_op->opb);
	free(tmp);
}

int
interpret_bin_op (struct AstNode *n,
									struct InterpreterCtx *ictx)
{
	int rv = 0x7FFFFFFF;
	int opa = interpret_ast_node(n->bin_op->opa, ictx);
	int opb = interpret_ast_node(n->bin_op->opb, ictx);
	char *op = n->bin_op->op;
	if ( strncmp("+", op, 3) == 0 ) {
		rv = opa + opb;
	} else if ( strncmp("-", op, 3) == 0 ) {
		rv = opa - opb;
	} else if ( strncmp("*", op, 3) == 0 ) {
		rv = opa * opb;
	} else if ( strncmp("/", op, 3) == 0 ) {
		rv = opa / opb;
	} else if ( strncmp("<", op, 3) == 0 ) {
		rv = (opa < opb);
	} else if ( strncmp("<=", op, 3) == 0 ) {
		rv = (opa <= opb);
	} else if ( strncmp(">", op, 3) == 0 ) {
		rv = (opa > opb);
	} else if ( strncmp(">=", op, 3) == 0 ) {
		rv = (opa >= opb);
	} else if ( strncmp("==", op, 3) == 0 ) {
		rv = (opa == opb);
	} else if ( strncmp("!=", op, 3) == 0 ) {
		rv = (opa != opb);
	} else {
		throw_interpreter_error("Unknown operation!", op);
	}
	return rv;
}
