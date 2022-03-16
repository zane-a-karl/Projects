#ifndef _BINARY_OP_H_
#define _BINARY_OP_H_

typedef enum OperandType {
  CONSTANT, REGISTER, VARIABLE
} OperandType;

// A struct to store the outcome of an individual
// designator or number parse.
typedef struct Operand {
  OperandType type;
  int value; // This could be an immediate value, an offset
             //of a register, or an address of a variable
	char *raw_str;
} Operand;

typedef struct BinaryOperation {
	Operand opa;
	char operator;
	Operand opb;
} BinOp;

BinOp *
init_bin_op ();

Operand *
init_operand ();

#endif//_BINARY_OP_H_
