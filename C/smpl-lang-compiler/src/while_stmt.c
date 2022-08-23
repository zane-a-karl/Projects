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
