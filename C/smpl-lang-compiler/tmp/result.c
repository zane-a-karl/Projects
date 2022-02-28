#include "../hdr/result.h"

void
compute (int op_code,
				 Result *x,
				 Result *y) {
	if (x->kind == CONST && y->kind == CONST) {
		if (op_code == 0) { // ADD
			x->imm += y->imm;
		} else if (op_code == 2) { // MUL
			x->imm *= y->imm;
		}
	} else {
		load(x);
		if (y->kind == CONST) {
			putF1(op_codes[op_code + 16], x->reg, x->reg, y->imm);
		} else {
			load(y);
			putF1(op_codes[op_code], x->reg, x->reg, y->reg);
			deallocateRegister(y->reg);
		}
	}
}

void
load (Result *x) {
	x->reg = allocateRegister();
	if (x->reg == -1) {
		printf("Out of space in memory!!!\n");
		exit(0);
	}
	if (x->kind == CONST) {
		putF1(op_codes[16], x->reg, memory[0].data, x->imm); // ADDI
	} else if (x->kind == VARIABLE) {
		putF2(op_codes[32], x->reg, memory[30].data, x->addr); //LDW
	}
	x->kind = REGISTER;
}

int
allocateRegister () {
	for (int i = 0; i < NUM_REGISTERS; ++i) {
		if (memory[i].unassigned) {
			memory[i].unassigned = false;
			return i;
		}
	}
	return -1;
}

void
deallocateRegister (int r) {
	memory[r].unassigned = true;
	memory[r].data = 0;
}

void
putF1 (const char *op_code,
			 int operand1,
			 int operand2,
			 int operand3) {

	printf("%s %i, %i, %i\n",
				 op_code,
				 operand1,
				 operand2,
				 operand3);
}

void
putF2 (const char *op_code,
			 int operand1,
			 int operand2,
			 int operand3) {
	printf("%s %i, %i, %i\n",
				 op_code,
				 operand1,
				 operand2,
				 operand3);
}
