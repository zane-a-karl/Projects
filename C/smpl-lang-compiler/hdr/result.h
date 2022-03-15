#ifndef _RESULT_H_
#define _RESULT_H_

typedef enum ResultType {
	CONSTANT, REGISTER, VARIABLE
} ResultType;

// A struct to store the outcome of an individual
// designator or number parse.
typedef struct Result {
	ResultType type;
	int data; // This could be an immediate value, an offset
           	//of a register, or an address of a variable
} Result;

void
set_result (Result *r,
						ResultType t,
						TokenNode *tn);

#endif//_RESULT_H_
