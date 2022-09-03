#include "../hdr/operand.h"

struct Operand *
new_operand (enum OperandType type)
{
	struct Operand *op = calloc(1, sizeof(struct Operand));
	op->next           = NULL;
	switch ( type ) {
	case 	IMMEDIATE:
		op->immediate->val = 0x7FFFFFFF;
		break;
	case UNINITVAR:
		op->uninit_var->name = calloc(MAX_VAR_NAME_LEN, sizeof(char));
		break;
	case INSTRUCTION:
		op->instruction->val = 0x7FFFFFFF;
		break;
	case ARGUMENT:
		op->argument->name = calloc(MAX_VAR_NAME_LEN, sizeof(char));
		break;
	case LABEL:
		op->argument->label = calloc(MAX_VAR_NAME_LEN, sizeof(char));
		break;
	case FUNCTION:
		op->function->name = calloc(MAX_VAR_NAME_LEN, sizeof(char));
		break;
	case POSSPHI:
		op->operand = NULL;
		break;
	default:
		perror("Unrecognized operand type: ", type);
		exit(1);
		break;
	}
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

