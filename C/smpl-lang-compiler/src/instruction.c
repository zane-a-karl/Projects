#include "../hdr/instruction.h"

struct Instruction *
new_instruction (char *name,
								 bool produces_output)
{
	struct Instruction *i = calloc(1, sizeof(struct Instruction));
	i->name               = deep_copy_str(name);
	i->ops                = NULL;
	i->i_num              = -1;
	i->produces_output    = produces_output;
	i->dominators         = NULL;
	i->next               = NULL;

	return i;
}

struct InstructionList *
new_instruction_list ()
{
	struct InstructionList *il = calloc(1, sizeof(InstructionList));
	il->head                   = NULL;

	return il;
}

// assume `new_instr` already calloc'd
void
push_instruction (struct InstructionList *il,
									struct Instruction     *new_i)
{
	struct Instruction *i = il->head;
	if ( i == NULL ) {
		il->head = new_i;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_i;
	}
}

void
free_instruction (struct Instruction **instr)
{
	free((*instr)->name);
	free_operand_list(&((*instr)->ops));
	//don't free dominator b/c it'll be freed elsewhere
	//don't free next b/c it'll be freed elsewhere
}

void
free_instruction_list (struct InstructionList **il)
{
	struct Instruction *cur = (*il)->head;
	struct Instruction *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur = cur->next;
		free_instruction(&prv);
	}
	free(*il);
}
