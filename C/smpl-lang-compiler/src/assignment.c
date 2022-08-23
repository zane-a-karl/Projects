#include "../hdr/assignment.h"

extern char *ast_node_types[];

struct Assignment *
new_assignment ()
{
	struct Assignment *a = calloc(1, sizeof(struct Assignment));
	a->lhs               = NULL;
	a->rhs               = NULL;

	return a;
}

struct Assignment *
deep_copy_assignment (struct Assignment *src)
{
	struct Assignment *dst = new_assignment();
	dst->lhs = deep_copy_ast_node(src->lhs);
	dst->rhs = deep_copy_ast_node(src->rhs);

	return dst;
}

void
create_assignment_agedge_set (char *label,
															int len,
															struct AstNode *n)
{
	Agedge_t *edge;
	agset(n->self, "label", ast_node_types[ASSMT]);
	edge = agedge(n->graph, n->self, n->assignment->lhs->self, "LHS", TRUE);
	agset(edge, "label", "LHS");
	create_agedge_set(n->assignment->lhs);
	edge = agedge(n->graph, n->self, n->assignment->rhs->self, "RHS", TRUE);
	agset(edge, "label", "RHS");
	create_agedge_set(n->assignment->rhs);
}

int
interpret_assignment (struct AstNode *n,
											struct InterpreterCtx *ictx)
{
	int rhs = 0, idx = 0;
	char *name = NULL;
	struct AstNode *i;
	struct StrHashEntry *var_arr, *var_arr_dim;
	//Check if we're assigning to a var or an array
	if ( n->assignment->lhs->type == IDNT ) {
		name = n->assignment->lhs->identifier->name;
	} else if ( n->assignment->lhs->type == ARRACC ) {
		name =
			n->assignment->lhs->arr_acc->ident->identifier->name;
	} else {
		throw_interpreter_error("Unknown var decl type: ", name);
	}
	//Check if the var/arry has been declared
	var_arr = sht_lookup(ictx->locals, name);
	if ( var_arr == NULL ) {
		throw_interpreter_error("Assignment to undelcared var ",
														name);
	}
	rhs = interpret_ast_node(n->assignment->rhs, ictx);
	//Perform the assignment to a var
	if ( n->assignment->lhs->type == IDNT ) {
		
		var_arr->data[0] = rhs;
		//Perform the assignment to an array		
	} else if ( n->assignment->lhs->type == ARRACC ) {

		int arrdims_len = 0, final_idx = 0;
		//Get the number of dimsenions of the array
		for (i = n->assignment->lhs->arr_acc->indices->head;
				 i != NULL; i = i->next) {
			arrdims_len++;
		}
		int k = 1;
		//Loop through indices e.g. [a][b][c] and compute
		// a*dim[k..len] + b*dim[k..len] + c*dim[k..len]
		var_arr_dim = sht_lookup(ictx->arrdims, name);
		for (i = n->assignment->lhs->arr_acc->indices->head;
				 i != NULL; i = i->next) {

			idx = interpret_ast_node(i, ictx);
			for (int j = k; j < arrdims_len; ++j) {
				idx *= var_arr_dim->data[j];
			}
			final_idx += idx;
			++k;
		}
		var_arr->data[final_idx] = rhs;
	} else {
		throw_interpreter_error("Unknown var decl type", name);
	}

	return 0x7FFFFFFF;
}
