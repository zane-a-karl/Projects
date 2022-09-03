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

struct Operand *
compile_bin_op (struct AstNode *n,
								struct CompilerCtx *cctx)
{
	struct Operand *a_op = compile_ast_node(n->bin_op->opa, cctx);
	struct Operand *b_op = compile_ast_node(n->bin_op->opb, cctx);
	char *op = n->bin_op->op;
	if ( strncmp(op, "+", 3) == 0 ) {
		return compiler_ctx_emit(cctx, "add", a_op, b_op, true);
	} else if ( strncmp(op, "-", 3) == 0 ) {
		return compiler_ctx_emit(cctx, "sub", a_op, b_op, true);
	} else if ( strncmp(op, "*", 3) == 0 ) {
		return compiler_ctx_emit(cctx, "mul", a_op, b_op, true);
	} else if ( strncmp(op, "/", 3) == 0 ) {
		return compiler_ctx_emit(cctx, "div", a_op, b_op, true);
	} else if ( strncmp(op, "<",  3) == 0 ||
							strncmp(op, "<=", 3) == 0 ||
							strncmp(op, ">",  3) == 0 ||
							strncmp(op, ">=", 3) == 0 ||
							strncmp(op, "==", 3) == 0 ||
							strncmp(op, "!=", 3) == 0 ) {
		return compiler_ctx_emit(cctx, "cmp", a_op, b_op, true);
	} else {
		throw_compiler_error("Unrecognized binary op: ", op);
		return NULL;
	}
}

/**
 * Compiles a conditional jump executed on the condition node `n`
 * for the OPPOSITE of that condition. I.e. the jump is only
 * performed if the condition is FALSE, otherwise it falls through.
 */
struct Operand *
compile_conditional_jump (struct AstNode     *n,
													struct CompilerCtx *cctx,
													struct BasicBlock  *bb)
{
	struct Operand *cond_op  = compile_ast_node(n, cctx);
	struct Operand *label_op = new_operand(LABEL);
	label_op->label->name = bb->label;
	char *op = n->bin_op->op;
	if ( strncmp(op, ">=", 3) == 0 ) {
		compiler_ctx_emit(cctx, "blt", cond_op, label_op, false);
	} else if ( strncmp(op, ">", 3) == 0 ) {
		compiler_ctx_emit(cctx, "ble", cond_op, label_op, false);
	} else if ( strncmp(op, "<=", 3) == 0 ) {
		compiler_ctx_emit(cctx, "bgt", cond_op, label_op, false);
	} else if ( strncmp(op, "<", 3) == 0 ) {
		compiler_ctx_emit(cctx, "bge", cond_op, label_op, false);
	} else if ( strncmp(op, "!=",  3) == 0 ) {
		compiler_ctx_emit(cctx, "beq", cond_op, label_op, false);
	} else if ( strncmp(op, "==",  3) == 0 ) {
		compiler_ctx_emit(cctx, "bne", cond_op, label_op, false);
	} else {
		throw_compiler_error("Unrecognized binary op: ", op);
	}
	return NULL;
}
