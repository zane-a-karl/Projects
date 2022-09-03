#include "../hdr/while_stmt.h"

extern char *ast_node_types[];

struct WhileStmt *
new_while_stmt ()
{
	struct WhileStmt *ws = calloc(1, sizeof(struct WhileStmt));
	ws->condition        = NULL;
	ws->do_stmts         = NULL;

	return ws;
}

struct WhileStmt *
deep_copy_while_stmt (struct WhileStmt *src)
{
	struct WhileStmt *dst = new_while_stmt();
	dst->condition = deep_copy_ast_node(src->condition);
	dst->do_stmts = deep_copy_ast_node_list(src->do_stmts);

	return dst;
}

void
create_while_stmt_agedge_set (char *label,
															int len,
															struct AstNode *n)
{
	Agedge_t *edge;
	agset(n->self, "label", ast_node_types[WHSTMT]);
	edge = agedge(n->graph, n->self, n->while_stmt->condition->self, NULL, TRUE);
	agset(edge, "label", "Condition");
	create_agedge_set(n->while_stmt->condition);
	for (struct AstNode *i = n->while_stmt->do_stmts->head; i != NULL; i = i->next) {

		edge = agedge(n->graph, n->self, i->self, NULL, TRUE);
		agset(edge, "label", "Loop");
		create_agedge_set(i);
	}
}

int
interpret_while_stmt (struct AstNode *n,
											struct InterpreterCtx *ictx)
{
	int rv;
	int cond;
	struct AstNode *i;
	while (1) {
		cond = interpret_ast_node(n->while_stmt->condition, ictx);
		if ( !cond ) {
			break;
		}
		for (i=n->while_stmt->do_stmts->head; i!=NULL; i=i->next) {
			rv = interpret_ast_node(i, ictx);
		}
	}
	return rv;
}

struct Operand *
compile_while_stmt (struct AstNode *n,
										struct CompilerCtx *cctx)
{
	struct BasicBlock *orig_block = cctx->cur_block;
	struct BasicBlock *head_block = new_basic_block(cctx);
	copy_block_ctx_params(head_block, cctx->cur_block);

	add_successor(cctx->cur_block, head_block);

	push_basic_block(cctx->cur_block->dominatees, head_block, DOMEES);

	struct BasicBlock *body_block = new_basic_block(cctx);
	copy_block_ctx_params(body_block, cctx->cur_block);
	struct Operand *orig_op;
	struct StrHashEntry *j;
	int k;
	for (k = 0; k < MAX_NUM_VARS; ++k) {
		j = body_block->locals_op->entries[k];
		for (; j != NULL; j = j->next) {
			orig_op = j->operand;
			j->operand = new_operand(POSSPHI);
			j->operand->op = orig_op;
		}
	}
	cctx->cur_block = body_block;
	struct AstNode *i;
	i = n->if_stmt->then_stmts->head;
	for (; i != NULL; i = i->next) {
		compile_ast_node(i, cctx);
	}
	struct Operand *label_op = new_operand(LABEL);
	label_op->label->name = head_block->label;
	compiler_ctx_emit(cctx, "bra", label_op, false);
	struct BasicBlock *body_end_block = cctx->cur_block;

	cctx->cur_block = head_block;
	struct SomeOpContainer *head, *body;
	struct Operand *wrapped_head_op, *renamed_op;
	struct Operand *label_op1, label_op2;

	for (k = 0; k < MAX_NUM_VARS; ++k) {
		j = body_end_block->locals_op->entries[k];
		for (; j != NULL; j = j->next) {
			head            = get_local(head_block, j->name);
			wrapped_head_op = new_operand(POSSPHI);
			wrapped_head_op->poss_phi->op = head->val_op;
			body            = get_local(body_end_block, j->name);
			//TODO You need to check the names not the pointers!!
			if ( body->val_op != wrapped_head_op ) {

				label_op1 = new_operand(LABEL);
				label_op1->label->name = orig_block->label;
				label_op2 = new_operand(LABEL);
				label_op2->label->name = body_end_block->label;
				renamed_op = compiler_ctx_emit(cctx, "phi",
																			 label_op1, head->val_op,
																			 label_op2, body->val_op);
				rename_op(body_block, wrapped_head_op, renamed_op);
				set_local_op(head_block, j->name, renamed_op);
				set_local_op(body_block, j->name, renamed_op);
			} else {
				rename_op(body_block, wrapped_head_op, head->val_op);
			}//if
		}//j for
	}//k for
	struct BasicBlock *exit_block = new_basic_block(cctx);
	copy_block_ctx_params(exit_block, cctx->cur_block);
	compile_conditional_jump(n->while_stmt->condition, cctx, exit_block);

	add_successor(head_block, body_block);
	add_successor(head_block, exit_block);
	add_successor(body_end_block, head_block);

	push_basic_block(head_block->dominatees, body_block, DOMEES);
	push_basic_block(head_block->dominatees, exit_block, DOMEES);

	cctx->cur_block = exit_block;

	return NULL;
}
