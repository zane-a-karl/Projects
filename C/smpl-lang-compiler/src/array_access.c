#include "../hdr/array_access.h"

struct ArrayAccess *
new_array_access ()
{
	struct ArrayAccess *aa;
	aa          = calloc(1, sizeof(struct ArrayAccess));
	aa->ident   = NULL;
	aa->indices = new_ast_node_list();

	return aa;
}

struct ArrayAccess *
deep_copy_array_access (struct ArrayAccess *src)
{
	struct ArrayAccess *dst = new_array_access();
	dst->ident = deep_copy_ast_node(src->ident);
	dst->indices = deep_copy_ast_node_list(src->indices);

	return dst;
}

void
create_array_access_agedge_set (char *label,
																int len,
																struct AstNode *n)
{
	if ( n->arr_acc->indices->head != NULL ) {
		snprintf(label, len, "Array Access '%s'", n->arr_acc->ident->identifier->name);
	} else {
		snprintf(label, len, "Access '%s'", n->arr_acc->ident->identifier->name);
	}
	agdelnode(n->graph, n->arr_acc->ident->self);
	/* create_agedge_set(n->arr_acc->ident); */
	char *tmp = calloc(len, sizeof(char));
	Agedge_t *edge;
	int idx_num = 0;
	for (struct AstNode *i = n->arr_acc->indices->head; i != NULL; i = i->next) {

		snprintf(tmp, len, "%s", "[-]");
		strlcat(label, tmp, strlen(label) + strlen(tmp) + 1);
		edge = agedge(n->graph, n->self, i->self, tmp, TRUE);
		snprintf(tmp, len, "Idx#%d", idx_num++);
		agset(edge, "label", "");
		create_agedge_set(i);
	}
	agset(n->self, "label", label);
	free(tmp);
}

int
interpret_array_access (struct AstNode *n,
												struct InterpreterCtx *ictx)
{
	int rv = 0x7FFFFFFF, final_idx = 0;
	int idx, j, k;
	struct AstNode *i;
	char *name = n->arr_acc->ident->identifier->name;
	int arrdims_len = 0;	
	struct StrHashEntry *arrdims = sht_lookup(ictx->arrdims, name);
	struct StrHashEntry *arr = sht_lookup(ictx->locals, name);
	//Check if it exists
	if ( arr == NULL ) {
		throw_interpreter_error("Access to undeclared array: ",
														name);
	}
	for (i = n->arr_acc->indices->head; i != NULL; i = i->next) {
		arrdims_len++;
	}
	k = 1;
	//Loop through indices e.g. [a][b][c] and compute
	// a*dim[k..len] + b*dim[k..len] + c*dim[k..len]
	for (i = n->arr_acc->indices->head; i != NULL; i = i->next) {

		idx = interpret_ast_node(i, ictx);
		for (j = k; j < arrdims_len; ++j) {
			idx *= arrdims->data[j];
		}
		final_idx += idx;
		k++;
	}
	rv = arr->data[final_idx];
	if ( rv == 0x7FFFFFFF ) {
		throw_interpreter_warning("Access to uninit'd array idx ",
															name);
		arr->data[final_idx] = 0;
		rv = arr->data[final_idx];
	}
	return rv;
}
