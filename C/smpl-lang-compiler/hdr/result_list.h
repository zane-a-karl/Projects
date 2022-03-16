#ifndef _RESULT_H_
#define _RESULT_H_

typedef enum ResultType {
	CONSTANT, REGISTER, VARIABLE
} ResultType;

// A struct to store the outcome of an individual
// expression parse
typedef struct Result {
	ResultType type;
	int result;// This could be an immediate value, an	
             //offset of a register, or an address of a
	           //variable
} Result;

typedef struct ResultListNode {
	Result *data;
	ResultListNode *next;
} ResultListNode;

typedef struct ResultList {
	ResultListNode *head;
} ResultList;

void
set_result (Result *r,
						ResultType t,
						TokenNode *tn);

ResultList *
init_result_list ();

ResultListNode *
init_result_list_node (Result *data);

void
next_result_list_node (ResultListNode **rln);

void
push_result_list (ResultList **rl,
									ResultList *new_rl);

void
push_result_list_node (ResultList **rl,
											 ResultListNode *new_node);

void
push_result_list_data (ResultList **rl,
											 Result new_data);

ResultList *
deep_copy_result_list (ResultList *src_rl);//calloc

void
prresult_result_list (ResultList *rl);

void
free_result_list (ResultList **rl);

#endif//_RESULT_H_
