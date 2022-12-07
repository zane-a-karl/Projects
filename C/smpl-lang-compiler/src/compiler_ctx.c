#include "../hdr/compiler_ctx.h"

struct CompilerCtx *
new_compiler_ctx (bool cse_bool)
{
	struct CompilerCtx *cc;
	cc            = calloc(1, sizeof(struct CompilerCtx));
	cc->instr_ctr = 1; // to be like andre for now
	cc->block_ctr = 0;
	cc->cur_block = NULL;
	cc->roots     = new_basic_block_list();
	cc->exec_cse  = cse_bool;

	return cc;
}

struct Operand *
compiler_ctx_emit (struct CompilerCtx *cctx,
									 bool                produces_output,
									 bool                may_eliminate,
									 int                 n_args,
									 ...)
{
	struct Operand *result_op;
	bool po = produces_output;
	bool me = may_eliminate;
	char *instr_name;
	va_list args;
	va_start(args, n_args);
	instr_name = va_arg(args, char *);//First arg is always the name
	result_op = vbasic_block_emit(cctx->cur_block, cctx->instr_ctr,
																instr_name, po, me, n_args-1,
																args);
	va_end(args);
	cctx->instr_ctr++;

	return result_op;
}

void
throw_compiler_error (char *err,
											char *reason)
{
	int len = strlen(err) + strlen(reason) + 1;
	char *output = calloc(len, sizeof(char));
	snprintf(output, len, "%s", err);
	strlcat(output, reason, len);
	perror("ERROR");
	perror(output);
	free(reason);
	free(output);
	exit(1);
}

void
throw_compiler_warning (char *warn,
												char *reason)
{
	int len = strlen(warn) + strlen(reason) + 1;
	char *output = calloc(len, sizeof(char));
	snprintf(output, len, "%s", warn);
	strlcat(output, reason, len);
	perror("WARNING");
	perror(output);
	free(output);
}

struct CompilerCtx *
compile (struct Ast *ast,
				 bool cse_bool)
{
	struct CompilerCtx *ir = new_compiler_ctx(cse_bool);
	compile_ast_node(ast->root, ir);

	return ir;
}

void
free_compiler_ctx (struct CompilerCtx **cctx)
{
	free_roots_basic_block_list(&((*cctx)->roots));
	//Don't free cur_block as it will be freed elsewhere
	free(*cctx);
}
