#include "../hdr/ast.h"

char *ast_node_types[] = {
	"Identifier",
	"Number",
	"Designator",
	"VariableDeclaration",
	"FunctionDeclaration",
	"Computation",
	"BinaryOperation",
	"Assignment",
	"FunctionCall",
	"IfStatement",
	"WhileStatement",
	"ReturnStatement"
};

struct Ast *
new_ast (Agraph_t *tlg)
{
	struct Ast *ast = calloc(1, sizeof(struct Ast));
	ast->graph      = agsubg(tlg, "cluster_AST", TRUE);
	ast->root       = NULL;

	return ast;
}

struct AstNode *
new_ast_node (Agraph_t *graph,
							int       node_number,
							enum AstNodeType type)
{
	struct AstNode *node = calloc(1, sizeof(struct AstNode));
	node->graph          = graph;

	char *default_name   = calloc(32, sizeof(char));
	snprintf(default_name, 32, "%i", node_number);
	node->self         = agnode(graph, default_name, TRUE);
	agset(node->self, "label", default_name);
	free(default_name);

	node->node_number    = node_number;
	node->next           = NULL;
	node->type           = type;

	return node;
}

struct AstNodeList *
new_ast_node_list ()
{
	struct AstNodeList *anl;
	anl       = calloc(1, sizeof(struct AstNodeList));
	anl->head = NULL;

	return anl;
}

struct AstNode *
deep_copy_ast_node (struct AstNode *src)
{
	struct AstNode *dst =
		new_ast_node(src->graph,
								 src->node_number + 1,
								 src->type);
	switch (src->type) {
	case 	IDNT:
		dst->identifier = deep_copy_ident(src->identifier);
		break;
	case NUM:
		dst->number = deep_copy_number(src->number);
		break;
	case DSGNTR:
		dst->designator = deep_copy_designator(src->designator);
		break;
  case VARDECL:
		dst->var_decl = deep_copy_var_decl(src->var_decl);
		break;
	case FUNCDECL:
		dst->func_decl = deep_copy_func_decl(src->func_decl);
		break;
	case CMPTN:
		dst->computation = deep_copy_computation(src->computation);
		break;
	case BINOP:
		dst->bin_op = deep_copy_bin_op(src->bin_op);
		break;
	case ASSMT:
		dst->assignment = deep_copy_assignment(src->assignment);
		break;
	case FUNCCALL:
		dst->func_call = deep_copy_func_call(src->func_call);
		break;
	case IFSTMT:
		dst->if_stmt = deep_copy_if_stmt(src->if_stmt);
		break;
	case WHSTMT:
		dst->while_stmt = deep_copy_while_stmt(src->while_stmt);
		break;
	case RESTMT:
		dst->ret_stmt = deep_copy_return_stmt(src->ret_stmt);
		break;
	default:
		perror("(deep_copy_ast_node): Unknown ast node type");
		exit(1);
		break;
	}

	return dst;
}

struct AstNodeList *
deep_copy_ast_node_list (struct AstNodeList *src)
{
	struct AstNodeList *dst = new_ast_node_list();
	for (struct AstNode *i = src->head;
			 i != NULL;
			 i = i->next) {
		push_ast_node(dst, deep_copy_ast_node(i));
	}
	return dst;
}

// assume `new_node` already calloc'd
void
push_ast_node (struct AstNodeList *anl,
							 struct AstNode *new_node)
{
	struct AstNode *an = anl->head;
	if ( an == NULL ) {
		anl->head = new_node;
	} else {
		while ( an->next != NULL ) {
			an = an->next;
		}
		an->next = new_node;
	}
}

/**
 * `new` will not be NULL, and will have at least one node.
 */
void
concat_ast_list (struct AstNodeList *anl,
								 struct AstNodeList *new_list)
{
	struct AstNode *an = anl->head;
	if ( an == NULL ) {
		anl->head = new_list->head;
		return;
	}
	while ( an->next != NULL ) {
		an = an->next;
	}
	an->next = new_list->head;
}

void
create_ident_agedge_set (char *label,
												 int len,
												 struct AstNode *n)
{
	snprintf(label, len, "%s", n->identifier->name);
	agset(n->self, "label", label);
}

void
create_num_agedge_set (char *label,
											 int len,
											 struct AstNode *n)
{
	snprintf(label, len, "%i", n->number->val);
	agset(n->self, "label", label);
}

void
create_designator_agedge_set (char *label,
															int len,
															struct AstNode *n)
{
	if ( n->designator->indices->head != NULL ) {
		snprintf(label, len, "Array Access '%s'", n->designator->ident->identifier->name);
	} else {
		snprintf(label, len, "Access '%s'", n->designator->ident->identifier->name);
	}
	agdelnode(n->graph, n->designator->ident->self);
	/* create_agedge_set(n->designator->ident); */
	char *tmp = calloc(len, sizeof(char));
	Agedge_t *edge;
	int idx_num = 0;
	for (struct AstNode *i = n->designator->indices->head; i != NULL; i = i->next) {

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

void
create_func_decl_agedge_set (char *label,
														 int len,
														 struct AstNode *n)
{
	Agedge_t *edge;
	snprintf(label, len, "Function '%s'", n->func_decl->fn_ident->identifier->name);
	/* create_agedge_set(n->func_decl->fn_ident); */
	agdelnode(n->graph, n->func_decl->fn_ident->self);
	agset(n->self, "label", label);
	for (struct AstNode *i = n->func_decl->param_idents->head; i != NULL; i = i->next) {

		edge = agedge(n->graph, n->self, i->self, "", TRUE);
		agset(edge, "label", "Param");
		create_agedge_set(i);
	}
	for (struct AstNode *i = n->func_decl->body->local_vars->head; i != NULL; i = i->next) {

		edge = agedge(n->graph, n->self, i->self, "", TRUE);
		agset(edge, "label", "Local Var");
		create_agedge_set(i);
	}
	for (struct AstNode *i = n->func_decl->body->stmts->head; i != NULL; i = i->next) {

		edge = agedge(n->graph, n->self, i->self, "", TRUE);
		agset(edge, "label", "Body");
		create_agedge_set(i);
	}
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

void
create_func_call_agedge_set (char *label,
														 int len,
														 struct AstNode *n)
{
	Agedge_t *edge;
	snprintf(label, len, "Call '%s'", n->func_call->ident->identifier->name);
	agset(n->self, "label", label);
	/* create_agedge_set(n->func_call->ident); */
	agdelnode(n->graph, n->func_call->ident->self);
	for (struct AstNode *i = n->func_call->args->head; i != NULL; i = i->next) {

		edge = agedge(n->graph, n->self, i->self, NULL, TRUE);
		agset(edge, "label", "Argument");
		create_agedge_set(i);
	}
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

void
create_return_stmt_agedge_set (char *label,
															 int len,
															 struct AstNode *n)
{
	Agedge_t *edge;
	agset(n->self, "label", ast_node_types[RESTMT]);
	edge = agedge(n->graph, n->self, n->ret_stmt->ret_val->self, NULL, TRUE);
	agset(edge, "label", "Return Value");
	create_agedge_set(n->ret_stmt->ret_val);
}

/**
 * Note agdelnode only deletes a node from the sub graph
 * and not from the top level graph!
 */
void
create_agedge_set (struct AstNode *n)
{
	int len = 32;
	char *label = calloc(len, sizeof(char));
	if ( n == NULL )
		return;

	switch (n->type) {
	case IDNT:
		create_ident_agedge_set(label, len, n);
		break;
	case NUM:
		create_num_agedge_set(label, len, n);
		break;
	case DSGNTR:
		create_designator_agedge_set(label, len, n);
		break;
  case VARDECL:
		create_var_decl_agedge_set(label, len, n);
		break;
	case FUNCDECL:
		create_func_decl_agedge_set(label, len, n);
		break;
	case CMPTN:
		create_computation_agedge_set(label, len, n);
		break;
	case BINOP:
		create_bin_op_agedge_set(label, len, n);
		break;
	case ASSMT:
		create_assignment_agedge_set(label, len, n);
		break;
	case FUNCCALL:
		create_func_call_agedge_set(label, len, n);
		break;
	case IFSTMT:
		create_if_stmt_agedge_set(label, len, n);
		break;
	case WHSTMT:
		create_while_stmt_agedge_set(label, len, n);
		break;
	case RESTMT:
		create_return_stmt_agedge_set(label, len, n);
		break;
	default:
		perror("(create_agedge_set): Unknown ast node type");
		exit(1);
		break;
	}
	free(label);
}

void
free_ast_node (struct AstNode *node)
{
	switch (node->type) {
	case 	IDNT:
		free(node->identifier->name);
		free(node->identifier);
		break;
	case NUM:
		free(node->number);
		break;
	case DSGNTR:
		free_ast_node(node->designator->ident);
		free_ast_node_list(node->designator->indices);
		free(node->designator);
		break;
  case VARDECL:
		free_ast_node(node->var_decl->ident);
		free_ast_node_list(node->var_decl->dims);
		free(node->var_decl);
		break;
	case FUNCDECL:
		free_ast_node(node->func_decl->fn_ident);
		free_ast_node_list(node->func_decl->param_idents);
		free_ast_node_list(node->func_decl->body->local_vars);
		free_ast_node_list(node->func_decl->body->stmts);
		free(node->func_decl->body);
		free(node->func_decl);
		break;
	case CMPTN:
		free_ast_node_list(node->computation->var_decls);
		free_ast_node_list(node->computation->func_decls);
		free_ast_node_list(node->computation->stmts);
		free(node->computation);
		break;
	case BINOP:
		free_ast_node(node->bin_op->opa);
		free(node->bin_op->op);
		free_ast_node(node->bin_op->opb);
		free(node->bin_op);
		break;
	case ASSMT:
		free_ast_node(node->assignment->lhs);
		free_ast_node(node->assignment->rhs);
		free(node->assignment);
		break;
	case FUNCCALL:
		free_ast_node(node->func_call->ident);
		free_ast_node_list(node->func_call->args);
		free(node->func_call);
		break;
	case IFSTMT:
		free_ast_node(node->if_stmt->condition);
		free_ast_node_list(node->if_stmt->then_stmts);
		free_ast_node_list(node->if_stmt->else_stmts);
		free(node->if_stmt);
		break;
	case WHSTMT:
		free_ast_node(node->while_stmt->condition);
		free_ast_node_list(node->while_stmt->do_stmts);
		free(node->while_stmt);
		break;
	case RESTMT:
		free_ast_node(node->ret_stmt->ret_val);
		free(node->ret_stmt);
		break;
	default:
		perror("(free_ast_node): Unknown ast node type");
		exit(1);
		break;
	}
}

void
free_ast_node_list (struct AstNodeList *anl)
{
	struct AstNode *cur = anl->head;
	struct AstNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free_ast_node(prv);
	}
}

void
free_ast (struct Ast **ast)
{
	free((*ast)->root);
	agclose((*ast)->graph);
	free((*ast));
}
