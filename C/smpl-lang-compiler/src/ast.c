#include "../hdr/ast.h"

char *ast_node_types[] = {
	"Identifier",
	"Number",
	"ArrayAccess",
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
	case ARRACC:
		dst->arr_acc = deep_copy_array_access(src->arr_acc);
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
	for (struct AstNode *i = src->head; i != NULL; i = i->next) {
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
 * `new_list` will not be NULL, and will have at least one node.
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
	case ARRACC:
		create_array_access_agedge_set(label, len, n);
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

int
interpret_ast_node (struct AstNode *n,
										struct InterpreterCtx *ictx)// mutated
{
	if ( n == NULL ) {
		return 0x7FFFFFFF;
	}

	switch (n->type) {
	case IDNT:
		return interpret_identifier(n, ictx);
	case NUM:
		return interpret_number(n, ictx);
	case ARRACC:
		return interpret_array_access(n, ictx);
	case VARDECL:
		return interpret_var_decl(n, ictx);
	case FUNCDECL:
		return interpret_func_decl(n, ictx);
	case CMPTN:
		return interpret_computation(n, ictx);
	case BINOP:
		return interpret_bin_op(n, ictx);
	case ASSMT:
		return interpret_assignment(n, ictx);
	case FUNCCALL:
		return interpret_func_call(n, ictx);
	case IFSTMT:
		return interpret_if_stmt(n, ictx);
	case WHSTMT:
		return interpret_while_stmt(n, ictx);
	case RESTMT:
		return interpret_return_stmt(n, ictx);
	default:
		perror("(interpret_ast_node): Unknown ast node type");
		exit(1);
		return 0x7FFFFFFF;
	}
}

int
interpret_ast (struct Ast *ast)
{
	struct InterpreterCtx *ictx = new_interpreter_ctx();
	int rv = interpret_ast_node(ast->root, ictx);
	printf("Program exited with code %i\n", rv);
	free_interpreter_ctx(&ictx);

	return rv;
}

struct Operand *
compile_ast_node (struct AstNode *n,
									struct CompilerCtx *cctx)
{
	if ( n == NULL ) {
		return NULL;
	}

	switch (n->type) {
	case IDNT:
		return compile_identifier(n, cctx);
	case NUM:
		return compile_number(n, cctx);
	case ARRACC:
		return compile_array_access(n, cctx);
	case VARDECL:
		return compile_var_decl(n, cctx);
	case FUNCDECL:
		return compile_func_decl(n, cctx);
	case CMPTN:
		return compile_computation(n, cctx);
	case BINOP:
		return compile_bin_op(n, cctx);
	case ASSMT:
		return compile_assignment(n, cctx);
	case FUNCCALL:
		return compile_func_call(n, cctx);
	case IFSTMT:
		return compile_if_stmt(n, cctx);
	case WHSTMT:
		return compile_while_stmt(n, cctx);
	case RESTMT:
		return compile_return_stmt(n, cctx);
	default:
		perror("(compile_ast_node): Unknown ast node type");
		exit(1);
		return NULL;
	}
}

void
free_ast_node (struct AstNode **node)
{
	switch ((*node)->type) {
	case 	IDNT:
		free((*node)->identifier->name);
		free((*node)->identifier);
		break;
	case NUM:
		free((*node)->number);
		break;
	case ARRACC:
		free_ast_node(&(*node)->arr_acc->ident);
		free_ast_node_list(&(*node)->arr_acc->indices);
		free((*node)->arr_acc);
		break;
  case VARDECL:
		free_ast_node(&(*node)->var_decl->ident);
		free_ast_node_list(&(*node)->var_decl->dims);
		free((*node)->var_decl);
		break;
	case FUNCDECL:
		free_ast_node(&(*node)->func_decl->fn_ident);
		free_ast_node_list(&(*node)->func_decl->param_idents);
		free_ast_node_list(&(*node)->func_decl->body->local_vars);
		free_ast_node_list(&(*node)->func_decl->body->stmts);
		free((*node)->func_decl->body);
		free((*node)->func_decl);
		break;
	case CMPTN:
		free_ast_node_list(&(*node)->computation->var_decls);
		free_ast_node_list(&(*node)->computation->func_decls);
		free_ast_node_list(&(*node)->computation->stmts);
		free((*node)->computation);
		break;
	case BINOP:
		free_ast_node(&(*node)->bin_op->opa);
		free((*node)->bin_op->op);
		free_ast_node(&(*node)->bin_op->opb);
		free((*node)->bin_op);
		break;
	case ASSMT:
		free_ast_node(&(*node)->assignment->lhs);
		free_ast_node(&(*node)->assignment->rhs);
		free((*node)->assignment);
		break;
	case FUNCCALL:
		free_ast_node(&(*node)->func_call->ident);
		free_ast_node_list(&(*node)->func_call->args);
		free((*node)->func_call);
		break;
	case IFSTMT:
		free_ast_node(&(*node)->if_stmt->condition);
		free_ast_node_list(&(*node)->if_stmt->then_stmts);
		free_ast_node_list(&(*node)->if_stmt->else_stmts);
		free((*node)->if_stmt);
		break;
	case WHSTMT:
		free_ast_node(&(*node)->while_stmt->condition);
		free_ast_node_list(&(*node)->while_stmt->do_stmts);
		free((*node)->while_stmt);
		break;
	case RESTMT:
		free_ast_node(&(*node)->ret_stmt->ret_val);
		free((*node)->ret_stmt);
		break;
	default:
		perror("(free_ast_node): Unknown ast node type");
		exit(1);
		break;
	}
	free(*node);
}

void
free_ast_node_list (struct AstNodeList **anl)
{
	struct AstNode *cur = (*anl)->head;
	struct AstNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur = cur->next;
		free_ast_node(&prv);
	}
	free(*anl);
}

void
free_ast (struct Ast **ast)
{
	free((*ast)->root);
	agclose((*ast)->graph);
	free((*ast));
}
