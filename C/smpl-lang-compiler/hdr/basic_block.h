#ifndef _BASIC_BLOCK_H_
#define _BASIC_BLOCK_H_

#include "../hdr/ast.h"
#include "../hdr/str_hash_table.h"
#include "../hdr/str_list.h"
#include "../hdr/instruction.h"
#include "../hdr/operation.h"
#include "../hdr/constants.h"

#include <stdbool.h>
#include <stdio.h>

enum BasicBlockListType {
	ROOTS,
	SUCCS,
	PREDS,
	DOMEES
};


//Necessary Forward Declaration
struct BasicBlockList;
struct BlockGroup;

// I'm pretty sure I need more than one `next` pointer b/c
// I have lists of blocks that may have some of the same blocks
// within them. I think I got away with a single next pointer
// in my previous lists because the list nodes never
// intersected.
struct BasicBlock {
	struct InstructionList *instrs;
	int                     instrs_len;
	struct Instruction     *latest_instr;
	char                   *label;
	struct BasicBlockList  *successors;
	struct BasicBlock      *next_s;
	struct BasicBlockList  *predecessors;
	struct BasicBlock      *next_p;
	struct StrHashTable    *locals_op;
	struct StrHashTable    *locals_dim;
	struct BlockGroup      *function;
	struct BasicBlockList  *dominatees;//BB's dom'd by this BB
	struct BasicBlock      *next_d;
	struct StrHashTable    *dom_instr_tree;
	struct BasicBlock      *next_r;//Points to next root BB
};

struct BasicBlockList {
	struct BasicBlock *head;
};

//A.k.a. a function, with arguments
struct BlockGroup {
	struct BasicBlock *entry;
	struct BasicBlock *exit;
	struct StrList    *arg_names;
	char              *name;
	bool               is_main;
};

struct BasicBlock *
new_basic_block (struct CompilerCtx *cctx);

struct BasicBlockList *
new_basic_block_list ();

struct BlockGroup *
new_block_group (char *name);

void
push_basic_block (struct BasicBlockList  *bbl,
									struct BasicBlock      *new_bb,
									enum BasicBlockListType type);

void
push_to_roots (struct BasicBlockList *bbl,
							 struct BasicBlock     *new_bb);

void
push_to_successors (struct BasicBlockList *bbl,
										struct BasicBlock     *new_bb);

void
push_to_predecessors (struct BasicBlockList *bbl,
											struct BasicBlock     *new_bb);

void
push_to_dominatees (struct BasicBlockList *bbl,
										struct BasicBlock     *new_bb);

struct Operand *
basic_block_emit (struct BasicBlock *bb,
									int                instr_num,
									char              *name,
									bool               produces_output,
									bool               exec_cse);

void
add_successor (struct BasicBlock *bb,
							 struct BasicBlock *successor_bb);

void
declare_local (struct BasicBlock *bb,
							 char              *name,
							 int               *dims);

struct SomeOpContainer
get_local (struct BasicBlock *bb,
					 char              *name);

void
set_local_op (struct BasicBlock *bb,
							char              *name,
							struct Operand    *val);

void
rename_op (struct BasicBlock *bb,
					 struct Operand    *old_op,
					 struct Operand    *new_op,
					 struct BasicBlock *visited);

void
copy_block_ctx_params (struct BasicBlock *dst,
											 struct BasicBLock *src);

#endif//_BASIC_BLOCK_H_
