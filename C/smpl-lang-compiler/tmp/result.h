#ifndef _RESULT_H_
#define _RESULT_H_

#include "../hdr/dlx_utils.h"

#include <stdio.h>
#include <stdlib.h>

typedef enum SubTreeCase {
	CONST, REGISTER, VARIABLE
} SubTreeCase;

typedef struct Result {
	SubTreeCase kind;
	int  imm; // if const
	int  reg; // if reg
	int addr; // if var
} Result;

void
compute (int op_code,
				 Result *x,
				 Result *y);

void
load (Result *x);

int
allocateRegister ();

void
deallocateRegister (int r);

void
putF1 (const char *op_code,
			 int operand1,
			 int operand2,
			 int operand3);

void
putF2 (const char *op_code,
			 int operand1,
			 int operand2,
			 int operand3);

#endif // _RESULT_H_
