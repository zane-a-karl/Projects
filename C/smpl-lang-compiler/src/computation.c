#include "../hdr/computation.h"

extern char *ast_node_types[];

struct Computation *
new_computation ()
{
	struct Computation *c;
	c             = calloc(1, sizeof(struct Computation));
	c->var_decls  = new_ast_node_list();
	c->func_decls = new_ast_node_list();
	c->stmts      = NULL;

	return c;
}

struct Computation *
deep_copy_computation (struct Computation *src)
{
	struct Computation *dst = new_computation();
	dst->var_decls = deep_copy_ast_node_list(src->var_decls);
	dst->func_decls = deep_copy_ast_node_list(src->func_decls);
	dst->stmts = deep_copy_ast_node_list(src->stmts);

	return dst;
}

void
create_computation_agedge_set (char *label,
															 int len,
															 struct AstNode *n)
{
	agset(n->self, "label", ast_node_types[CMPTN]);
	Agedge_t *edge;
	for (struct AstNode *i = n->computation->var_decls->head; i != NULL; i = i->next) {

		snprintf(label, len, "%s", ast_node_types[VARDECL]);
		edge = agedge(n->graph, n->self, i->self, NULL, TRUE);
		agset(edge, "label", label);
		agset(edge, "color", "blue");
		create_agedge_set(i);
	}
	memset(label, '\0', len*sizeof(char));
	for (struct AstNode *i = n->computation->func_decls->head; i != NULL; i = i->next) {

		snprintf(label, len, "%s", ast_node_types[FUNCDECL]);
		edge = agedge(n->graph, n->self, i->self, NULL, TRUE);
		agset(edge, "label", label);
		agset(edge, "color", "yellow");
		create_agedge_set(i);
	}
	memset(label, '\0', len*sizeof(char));
	for (struct AstNode *i = n->computation->stmts->head; i != NULL; i = i->next) {

		snprintf(label, len, "%s", "Statement");
		edge = agedge(n->graph, n->self, i->self, NULL, TRUE);
		agset(edge, "label", label);
		create_agedge_set(i);
	}
}

int
interpret_computation (struct AstNode *n,
											 struct InterpreterCtx *ictx)
{
	struct AstNode *i;
	int rv = 0;
	for (i=n->computation->var_decls->head; i!=NULL; i=i->next) {
		rv = interpret_ast_node(i, ictx);
	}
	for (i=n->computation->func_decls->head;i!=NULL;i=i->next) {
		rv = interpret_ast_node(i, ictx);
	}
	for (i=n->computation->stmts->head; i!=NULL; i=i->next) {

		rv = interpret_ast_node(i, ictx);
		if ( rv != 0x7FFFFFFF ) {
			break;
		}
	}
	return rv;
}
