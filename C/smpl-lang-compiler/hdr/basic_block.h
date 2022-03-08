#ifndef _BASIC_BLOCK_H_
#define _BASIC_BLOCK_H_

#include "dlx.h"

typedef struct BasicBlock {
	int blk_num;
	Instruction *i_list;
	struct BasicBlock *left;
	struct BasicBlock *right;	
} BasicBlock;

#endif//_BASIC_BLOCK_H_
