#include "../hdr/operand.h"

struct Operand *
new_operand (enum OperandType type,
						 ...)
{
	struct Operand *op = calloc(1, sizeof(struct Operand));
	op->next           = NULL;
	va_list args;
	va_start(args, type);
	switch ( type ) {
	case 	IMMEDIATE:
		op->immediate->val      = va_arg(args, int);
		break;
	case UNINITVAR:
		op->uninit_var->name    = va_arg(args, char *);
		break;
	case INSTRUCTION:
		op->instruction->number = va_arg(args, int);
		break;
	case ARGUMENT:
		op->argument->name      = va_arg(args, char *);
		break;
	case LABEL:
		op->label->name         = va_arg(args, char *);
		break;
	case FUNCTION:
		op->function->name      = va_arg(args, char *);
		break;
	case POSSPHI:
		op->poss_phi->op        = va_arg(args, struct Operand *);
		break;
	default:
		perror("Unrecognized operand type: ", type);
		exit(1);
		break;
	}
	va_end(args);
	return op;
}

struct OperandList *
new_operand_list ()
{
	struct OperandList *ol = calloc(1, sizeof(OperandList));
	ol->head               = NULL;

	return ol;
}

// assume `new_op` already calloc'd
void
push_operand (struct OperandList *ol,
							struct Operand     *new_op)
{
	struct Operand *o = ol->head;
	if ( o == NULL ) {
		ol->head = new_op;
	} else {
		while ( o->next != NULL ) {
			o = o->next;
		}
		o->next = new_op;
	}
}

bool
eq_operands (struct Operand *op1,
						 struct Operand *op2)
{
	if ( op1->type != op2->type ) {
		return false;
	} else {
		int n = MAX_VAR_NAME_LEN;
		switch ( op1->type ) {
		case 	IMMEDIATE:
			return op1->immediate->val == op2->immediate->val;
		case UNINITVAR:
			return strncmp(op1->uninit_var->name, op2->uninit_var->name, n);
		case INSTRUCTION:
			return op1->instruction->val == op2->instruction->val;
		case ARGUMENT:
			return strncmp(op1->argument->label, op2->argument->name, n);
		case LABEL:
			return strncmp(op1->label->name, op2->label->name, n);
		case FUNCTION:
			return strncmp(op1->function->label, op2->function->name, n);
		case POSSPHI:
			return eq_operands(op1->operand, op2->operand);
		default:
			perror("Unrecognized operand type: ", type);
			exit(1);
			return false;
		}
	}
}

bool
eq_operand_lists (struct OperandList *l1,
									struct OperandList *l2)
{
	struct Operand *i1 = l1->head;
	struct Operand *i2 = l2->head;
	for (; i1 != NULL && i2 != NULL; i1 = i1->next, i2 = i2->next) {
		if ( cmp_operands(i1, i2) ) {
			continue;
		} else {
			return false;
		}
	}
	return true;
}

void
free_operand (struct Operand **op)
{
	switch ( (*op)->type ) {
	case 	IMMEDIATE:
		break;
	case UNINITVAR:
		free((*op)->uninit_var->name);
		break;
	case INSTRUCTION:
		break;
	case ARGUMENT:
		free((*op)->argument->name);
		break;
	case LABEL:
		free((*op)->argument->label);
		break;
	case FUNCTION:
		free((*op)->function->name);
		break;
	case POSSPHI:
		free_operand(&((*op)->operand));
		break;
	default:
		perror("Unrecognized operand type: ", (*op)->type);
		exit(1);
		break;
	}	
}

void
free_operand_list (struct OperandList **ol)
{
	struct Operand *cur = (*ol)->head;
	struct Operand *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur = cur->next;
		free_operand(&prv);
	}
	free(*ol);
}

