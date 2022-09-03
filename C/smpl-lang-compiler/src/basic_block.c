#include "../hdr/basic_block.h"


struct BasicBlock *
new_basic_block (struct CompilerCtx *cctx)
{
	cctx->block_ctr++;

	struct BasicBlock *bb = calloc(1, sizeof(BasicBlock));
	bb->instrs            = NULL;
	bb->label             = calloc(MAX_VAR_NAME_LEN, sizeof(char));
	snprintf(bb->label, MAX_VAR_NAME_LEN, "BB%i", cctx->block_ctr);
	bb->successors        = NULL;
	bb->next_s            = NULL;
	bb->predecessors      = NULL;
	bb->next_p            = NULL;
	bb->locals_op         = NULL;
	bb->locals_dim        = NULL;
	bb->function          = NULL;
	bb->dominatees        = NULL;
	bb->next_d            = NULL;
	bb->dom_instr_tree    = NULL;
	bb->next_r            = NULL;

	return bb;
}

struct BasicBlockList *
new_basic_block_list ()
{
	struct BasicBlockList *bbl;
	bbl       = calloc(1, sizeof(struct BasicBlockList));
	bbl->head = NULL;

	return bbl;
}

struct BlockGroup *
new_block_group (char *name)
{
	struct BlockGroup *bg = calloc(1, sizeof(struct BlockGroup));
	bg->entry             = NULL;
	bg->exit              = NULL;
	bg->arg_names         = new_str_list();
	bg->name              = deep_copy_str(name);
	bg->is_main           = false;
	}
}

/* assume `new_bb` already calloc'd
 * We have to use a switch statement to push to the different BB
 * lists b/c there is a chance that the lists may share BB's and
 * thus a single `next` pointer will not be enough to iterate
 * through the list but a `next` pointer per list is necessary. The
 * switch statement allows us to determine to which list we push.
 */
void
push_basic_block (struct BasicBlockList  *bbl,
									struct BasicBlock      *new_bb,
									enum BasicBlockListType type)
{
	switch (type) {
	case ROOTS:
		push_to_roots(bbl, new_bb);
		break;
	case SUCCS:
		push_to_successors(bbl, new_bb);
		break;
	case PREDS:
		push_to_predecessors(bbl, new_bb);
		break;
	case DOMEES:
		push_to_dominatees(bbl, new_bb);
		break;
	default:
		perror("(push_basic_block): Unknown bb list type");
		exit(1);
		break;
	}
}

// assume `new_bb` already calloc'd
void
push_to_roots (struct BasicBlockList *bbl,
							 struct BasicBlock     *new_bb)
{
	struct BasicBlock *bb = bbl->head;
	if ( bb == NULL ) {
		bbl->head = new_bb;
	} else {
		while ( bb->next_r != NULL ) {
			bb = bb->next_r;
		}
		bb->next_r = new_bb;
	}
}

// assume `new_bb` already calloc'd
void
push_to_successors (struct BasicBlockList *bbl,
										struct BasicBlock     *new_bb)
{
	struct BasicBlock *bb = bbl->head;
	if ( bb == NULL ) {
		bbl->head = new_bb;
	} else {
		while ( bb->next_s != NULL ) {
			bb = bb->next_s;
		}
		bb->next_s = new_bb;
	}
}

// assume `new_bb` already calloc'd
void
push_to_predecessors (struct BasicBlockList *bbl,
											struct BasicBlock     *new_bb)
{
	struct BasicBlock *bb = bbl->head;
	if ( bb == NULL ) {
		bbl->head = new_bb;
	} else {
		while ( bb->next_p != NULL ) {
			bb = bb->next_p;
		}
		bb->next_p = new_bb;
	}
}

// assume `new_bb` already calloc'd
void
push_to_dominatees (struct BasicBlockList *bbl,
										struct BasicBlock     *new_bb)
{
	struct BasicBlock *bb = bbl->head;
	if ( bb == NULL ) {
		bbl->head = new_bb;
	} else {
		while ( bb->next_d != NULL ) {
			bb = bb->next_d;
		}
		bb->next_d = new_bb;
	}
}


/**
 *TODO I think this fn needs to be variadic :/
 */
struct Operand *
basic_block_emit (struct BasicBlock *bb,
									int                instr_num,
									char              *name,
									bool               produces_output,
									bool               exec_cse)
{
	struct Operand *rv;
	struct Instruction *instr;
	instr = new_instruction(instr_name, produces_output);
	char *dominance_class_name = dominance_class(instr);
	struct StrHashEntry *dom_class = sht_lookup(bb->dom_instr_tree,
																							dominance_class_name);
	if ( dom_class != NULL ) {
		instr->dominator = dom_class->instruction;
	}
	struct Instruction *identical;
	if ( exec_cse == true ) {
		identical = find_dominating_identical(instr);// calloc'd w/n
		if ( identical != NULL ) {
			rv = new_operand(INSTRUCTION);
			rv->instr->idx = instr->number;
			return rv;
		} else if ( strncmp(instr->name, "load", 5) == 0 &&
								bb->instrs_len > 0 &&
								strncmp(bb->latest_instr->name, "adda", 5) == 0 &&
								instr->ops->head->instr->idx == bb->latest_instr->number) {
			struct Instruction *identical_adda;
			identical_adda = find_dominating_identical(bb->latest_instr);
			if ( identical_adda != NULL ) {
				struct OperationList *orig_ops = instr->ops;
				struct Operand *adda_op = new_operand(INSTRUCTION);
				adda_op->instr->idx = identical_adda->number;
				push_operand(instr->ops, adda_op);
				identical = find_dominating_identical(instr);
				if ( identical != NULL ) {
					delete_instruction(bb->instrs, bb->latest_instr);
					rv = new_operand(INSTRUCTION);
					rv->instr->idx = identical->number;
					return rv;
				} else {
					instr->ops = orig_ops;
				}//identical!=NULL
			}//identical_adda!=NULL
		}//long else if
	}//exec_cse
	instr->number = instr_num;
	push_instruction(bb->instrs, instr);
	dom_class->instruction = instr;
	rv = new_operand(INSTRUCTION);
	rv->instr->idx = instr->number;

	return rv;
}

void
add_successor (struct BasicBlock *bb,
							 struct BasicBlock *successor_bb) {

	push_basic_block(bb->successors, successor_bb, SUCCS);
	push_basic_block(successor_bb->predecessors, bb, PREDS);
}

/**
 * Declare a local var form this block and set it to an uninitialied
 * value.
 * Assume `dims` has already been calloc'd
 */
void
declare_local (struct BasicBlock *bb,
							 char              *name,
							 int               *dims)
{
	struct StrHashEntry *var = sht_lookup(bb->locals_op, name);
	if ( var == NULL ) {
		throw_compiler_error("Redclaration of local var: ", name);
	}
	var->data[0] = 0x7FFFFFFF;//My `None` value
	struct StrHashEntry *dimensions = sht_lookup(bb->locals_dim, name);
	dimensions->data = dims;
}

struct SomeOpContainer
get_local (struct BasicBlock *bb,
					 char              *name)
{
	struct StrHashEntry *var = sht_lookup(bb->locals_op, name);
	if ( var == NULL ) {
		throw_compiler_error("Access to undeclared local: ", name);
	}
	struct Operand *var_op     = var->op;
	struct StrHashEntry *dims = sht_lookup(bb->locals_dim, name);
	if ( var_op == NULL ) {
		return new_uninitialized_var_op(name);
	}
	struct SomeOpContainer tmp;
	tmp->val_op = var_op;
	tmp->dims = dims->data;
	return tmp;
}

void
set_local_op (struct BasicBlock *bb,
							char              *name,
							struct Operand    *val)
{
	struct StrHashEntry *local = sht_lookup(bb->locals_op, name);
	if ( local == NULL ) {
		throw_compiler_error("Access to undeclared local: ", name);
	}
	local->operand = val;
}

void
rename_op (struct BasicBlock *bb,
					 struct Operand    *old_op,
					 struct Operand    *new_op,
					 struct BasicBlock *visited)
{
	int visited_len = 0;
	if ( visited == NULL ) {
		visited = realloc(visited, 1*sizeof(BasicBlock));
	}
	for () {}
}

void
copy_block_ctx_params (struct BasicBlock *dst,
											 struct BasicBLock *src)
{
	//Should I deep copy all of these?!?!
	//I think I should if I want to free things at the end :/
	dst->locals_op      = src->locals_op;
	dst->locals_dim     = src->locals_dim;
	dst->dom_instr_tree = src->dom_instr_tree;
	dst->function       = src->function;
}
