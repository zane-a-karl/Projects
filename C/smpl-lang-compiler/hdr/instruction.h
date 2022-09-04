#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "../hdr/operand.h"
#include "../hdr/utils.h"//for `deep_copy_str()`

//Necessary Forward Declaration
struct InstructionList;

struct Instruction {
	char                   *name;
	struct OperationList   *ops;
	int                     number;//I.e. the instruction number
	bool                    produces_output;
	struct Instruction     *dominator;//Instr that dominates this one
	struct Instruction     *next;
};

struct InstructionList {
	struct Instruction *head;
};

struct Instruction *
new_instruction (bool produces_output);

struct InstructionList *
new_instruction_list ();

void
push_instruction (struct InstructionList *il,
									struct Instruction     *new_i);

void
delete_instruction (struct InstructionList *il,
										struct Instruction     *instr);

char *
dominance_class (struct Instruction *instr);

struct Instruction *
find_dominating_identical (struct Instruction *instr);

void
free_instruction (struct Instruction **instr);

void
free_instruction_list (struct InstructionList **il);

#endif//_INSTRUCTION_H_
