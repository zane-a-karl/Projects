#ifndef _OPERAND_H_
#define _OPERAND_H_

#include "../hdr/constants.h"

enum OperandType {
	IMMEDIATE,
	UNINITVAR,
	INSTRUCTION,
	ARGUMENT,
	LABEL,
	FUNCTION,
	POSSPHI
};

struct ImmediateOp {
	int val;
};
struct UninitializedVarOp {
	char *name;
};
struct InstructionOp {
	int number;
};
struct ArgumentOp {
	char *name;
};
struct LabelOp {
	char *name;
};
struct FunctionOp {
	char *name;
};

//Necessary Forward Declaration?
struct Operand;

struct PossiblyPhiOp {
	struct Operand *op;
};

struct Operand {
	enum OperandType             type;
	union {
		struct ImmediateOp        *immediate;
		struct UninitializedVarOp *uninit_var;
		struct InstructionOp      *instrunction;
		struct ArgumentOp         *argument;
		struct LabelOp            *label;
		struct FunctionOp         *function;
		struct PossiblyPhiOp      *poss_phi;
	};
	struct Operand              *next;
};

struct OperandList {
	struct Operand *head;
};

struct Operand *
new_operand (enum OperandType type,
						 ...);

struct OperandList *
new_operand_list ();

void
push_operand (struct OperandList *ol,
							struct Operand     *new_op);

bool
eq_operands (struct Operand *op1,
						 struct Operand *op2);

bool
eq_operand_lists (struct OperandList *l1,
									struct OperandList *l2);

void
free_operand (struct Operand **op);

void
free_operand_list (struct OperandList **ol);

#endif//_OPERAND_H_
