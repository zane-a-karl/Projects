#include "../hdr/compiler_ctx.h"

struct CompilerCtx *
new_compiler_ctx (bool cse_bool)
{
	struct CompilerCtx *cc;
	cc            = calloc(1, sizeof(struct CompilerCtx));
	cc->instr_ctr = 0;
	cc->block_ctr = 0;
	cc->cur_block = NULL;	
	cc->roots     = new_basic_block_list();
	cc->exec_cse  = cse_bool;

	return cc;
}

void
compiler_ctx_emit (struct CompilerCtx *cctx,
									 char               *instr_name,
									 bool                produces_output)
{
	add_instruction_to_block(cctx->cur_block,
													 cctx->instr_ctr,
													 instr_name,
													 produces_output,
													 cctx->exec_cse);
	cctx->instr_ctr++;
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
throw_compiler_warning(char *warn,
											 char *reason)
{
	int len = strlen(warn) + strlen(reason) + 1;
	char *output = calloc(len,
												sizeof(char));
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
	struct CompilerCtx *cctx = new_compiler_ctx(cse_bool);
	compile_ast_node(ast->root);

	return ir;
}

void
free_compiler_ctx (struct CompilerCtx **cctx)
{
	free((*cctx));
}
