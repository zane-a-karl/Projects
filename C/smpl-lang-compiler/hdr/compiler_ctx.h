#ifndef _COMPILER_CTX_H_
#define _COMPILER_CTX_H_

#include "../hdr/ast.h"
#include "../hdr/basic_block.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct CompilerCtx {
	int                    instr_ctr;
	int                    block_ctr;
	struct BasicBlock     *cur_block;	
	struct BasicBlockList *roots;
	bool                   exec_cse;
};

struct CompilerCtx *
new_compiler_ctx (bool cse_bool);

void
add_instruction_to_ctx (struct CompilerCtx *cctx,
												char               *instr_name,
												bool                produces_output);

void
throw_compiler_error (char *err,
											char *reason);

void
throw_compiler_warning (char *warn,
												char *reason);

struct CompilerCtx *
compile (struct Ast *ast,
				 bool cse_bool);

void
free_compiler_ctx (struct CompilerCtx **cctx);

#endif//_COMPILER_CTX_H_
