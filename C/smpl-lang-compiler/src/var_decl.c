#include "../hdr/var_decl.h"

struct VarDecl *
new_vd ()
{
	struct VarDecl *vd = calloc(1, sizeof(struct VarDecl));
	vd->dims           = NULL;
	vd->ident          = NULL;

	return vd;
}

struct VarDecl *
deep_copy_var_decl (struct VarDecl *src)
{
	struct VarDecl *dst = new_vd();
	dst->ident          = deep_copy_ast_node(src->ident);
	dst->dims           = deep_copy_ast_node_list(src->dims);

	return dst;
}

void
create_var_decl_agedge_set (char *label,
														int len,
														struct AstNode *n)
{
	char *tmp = calloc(len, sizeof(char));
	snprintf(label, len, "Declare '%s'", n->var_decl->ident->identifier->name);
	/* create_agedge_set(n->var_decl->ident); */
	agdelnode(n->graph, n->var_decl->ident->self);
	for (struct AstNode *i = n->var_decl->dims->head; i != NULL; i = i->next) {

		snprintf(tmp, len, "[%i]", i->number->val);
		strlcat(label, tmp, strlen(label) + strlen(tmp) + 1);
		/* create_agedge_set(i); */
		agdelnode(n->graph, i->self);
	}
	free(tmp);
	agset(n->self, "label", label);
}

int
interpret_var_decl (struct AstNode *n,
										struct InterpreterCtx *ictx)
{
	int data_len, dim, arrdims_len = 0, j = 0;
	struct AstNode *i;
	//'name' freed during interpreter_ctx free
	char *name =
		deep_copy_str(n->var_decl->ident->identifier->name);
	//Check if it's a new variable
	if ( sht_lookup(ictx->locals, name) != NULL ) {
		throw_interpreter_error("Re-declaration of var: ", name);
	}
	//If new, add it to hash map
	struct StrHashEntry *var = new_str_hash_entry(name);
	sht_insert(ictx->locals, var);
	//Give it a base of 1 int of space
	data_len = 1;
	var->data = calloc(data_len, sizeof(int));
	var->data_len = data_len;
	//If it's an arr give it 1 int of space to save the arr dims
	struct StrHashEntry *arrdims;
	if ( n->var_decl->dims->head != NULL ) {
		arrdims = new_str_hash_entry(deep_copy_str(name));
		sht_insert(ictx->arrdims, arrdims);
		arrdims_len = 1;
		arrdims->data = calloc(arrdims_len, sizeof(int));
	}
	//If it's an arr give it the requisite space
	for (i = n->var_decl->dims->head; i != NULL; i = i->next) {

		dim = interpret_ast_node(i, ictx);
		data_len *= dim;
		var->data = realloc(var->data, data_len * sizeof(int));
		var->data_len = data_len;
		if ( i->next != NULL ) {
			arrdims->data = realloc(arrdims->data,
															(++arrdims_len) * sizeof(int));
		}
		arrdims->data[j++] = dim;
	}
	//Initialize the var to 0x7FFFFFFF=NULL
	for (j = 0; j < data_len; ++j) {
		var->data[j] = 0x7FFFFFFF;//a.k.a. NULL
	}
	//Use MAX of 'int' for NULL
	return 0x7FFFFFFF;
}
