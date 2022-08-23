#include "../hdr/if_stmt.h"

extern char *ast_node_types[];

struct IfStmt *
new_if_stmt ()
{
	struct IfStmt *is = calloc(1, sizeof(struct IfStmt));
	is->condition     = NULL;
	is->then_stmts    = NULL;
	is->else_stmts    = NULL;

	return is;
}

struct IfStmt *
deep_copy_if_stmt (struct IfStmt *src)
{
	struct IfStmt *dst = new_if_stmt();
	dst->condition = deep_copy_ast_node(src->condition);
	dst->then_stmts = deep_copy_ast_node_list(src->then_stmts);
	dst->else_stmts = deep_copy_ast_node_list(src->else_stmts);

	return dst;
}

void
create_if_stmt_agedge_set (char *label,
													 int len,
													 struct AstNode *n)
{
	agset(n->self, "label", ast_node_types[IFSTMT]);
	Agedge_t *edge;
	edge = agedge(n->graph, n->self, n->if_stmt->condition->self, NULL, TRUE);
	agset(edge, "label", "Condition");
	create_agedge_set(n->if_stmt->condition);
	for (struct AstNode *i = n->if_stmt->then_stmts->head; i != NULL; i = i->next) {

		edge = agedge(n->graph, n->self, i->self, NULL, TRUE);
		agset(edge, "label", "True");
		agset(edge, "color", "green");
		create_agedge_set(i);
	}
	if ( n->if_stmt->else_stmts != NULL ) {
		for (struct AstNode *i = n->if_stmt->else_stmts->head; i != NULL; i = i->next) {

			edge = agedge(n->graph, n->self, i->self, NULL, TRUE);
			agset(edge, "label", "False");
			agset(edge, "color", "red");
			create_agedge_set(i);
		}
	}
}

int
interpret_if_stmt (struct AstNode *n,
									 struct InterpreterCtx *ictx)
{
	int rv = 0x7FFFFFFF;
	struct AstNode *i;
	int cond = interpret_ast_node(n->if_stmt->condition, ictx);
	if ( cond ) {

		for (i=n->if_stmt->then_stmts->head; i!=NULL; i=i->next) {
			rv = interpret_ast_node(i, ictx);
		}
	} else {
		if ( n->if_stmt->else_stmts != NULL ) {
			for (i=n->if_stmt->else_stmts->head;
					 i!=NULL; i=i->next) {

				rv = interpret_ast_node(i, ictx);
			}
		}
	}
	return rv;
}
