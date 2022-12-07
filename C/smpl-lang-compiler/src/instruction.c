#include "../hdr/instruction.h"


/**
 *Assume that the `struct Operand *`s in `va_list args` are already
 * calloc'ed
 */
struct Instruction *
new_instruction (char   *name,
								 bool    produces_output,
								 int     n_args,
								 va_list args)
{
	struct Instruction *i = calloc(1, sizeof(struct Instruction));
	i->name               = deep_copy_str(name);

	i->ops                = new_operand_list();
	if ( n_args != 3 ) {
		for (int j = 0; j < n_args; ++j) {
			struct Operand *op = va_arg(args, struct Operand *);
			if ( op->next == NULL) {
				push_operand(i->ops, op);
			} else {
				printf("ERROR: mixing op lists!");
				exit(1);
			}
		}
	} else if ( n_args == 3 ) {
		// This first va_arg was originally a boolean flag hence the below comment
		// consume flag arg (bools promoted to ints)
		int n_fn_args = va_arg(args, int);
		struct Operand *op = va_arg(args, struct Operand *);
		if ( op->next == NULL) {
			push_operand(i->ops, op);
		} else {
			printf("ERROR: mixing op lists!");
			exit(1);
		}
		struct OperandList *list = va_arg(args, struct OperandList *);
		struct Operand *k = list->head;
		for (int j = 0; j < n_fn_args/* && k != NULL*/; ++j, k = k->next) {
			push_operand(i->ops, k);
		}
		// Do I need to free 'list'?!?!?!?!?!?!
	}
	
	i->number             = -1;
	i->produces_output    = produces_output;
	i->dominator          = NULL;
	i->next               = NULL;

	return i;
}

struct InstructionList *
new_instruction_list ()
{
	struct InstructionList *il;
	il       = calloc(1, sizeof(struct InstructionList));
	il->head = NULL;

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
delete_instruction (struct InstructionList *il,
										struct Instruction     *to_delete)
{
	if ( to_delete == il->head ) {
		free_instruction(&(il->head));
		il->head = NULL;
		return;
	}
	struct Instruction *prv = il->head;
	struct Instruction *cur = il->head->next;
	for (; cur != NULL; cur = cur->next) {
		if ( cur == to_delete ) {
			prv->next = cur->next;
			free_instruction(&cur);
			cur->next = NULL;
			cur = NULL;
			return;
		}
		prv = cur;
	}
}

char *
dominance_class (struct Instruction *instr)
{
	if ( strncmp(instr->name, "adda", 5) == 0 ) {
		return "add";
	} else if ( strncmp(instr->name, "store", 6) == 0 ) {
		return "load";
	} else {
		return instr->name;
	}
}

struct Instruction *
find_dominating_identical (struct Instruction *instr)
{
	struct Instruction *dom_instr = instr->dominator;
	struct Instruction *candidate = NULL;
	while ( dom_instr != NULL ) {

		if ( strncmp(dom_instr->name, "adda", 5) == 0 &&
				 strncmp(instr->name    , "adda", 5) == 0) {
			// do nothing
		}
		if ( strncmp(dom_instr->name, "store", 6) == 0 ) {
			break;
		}
		if ( eq_operand_lists(instr->ops, dom_instr->ops) ) {
			candidate = dom_instr;
			break;
		}
		dom_instr = dom_instr->dominator;
	}
	return candidate;
}

void
free_instruction (struct Instruction **instr)
{
	free((*instr)->name);
	(*instr)->name = NULL;
	free_operand_list(&((*instr)->ops));
	//don't free dominator b/c it'll be freed elsewhere
	//don't free next b/c it'll be freed elsewhere
	free(*instr);
	*instr = NULL;
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
	*il = NULL;
}
