#ifndef _RESULT_LIST_H_
#define _RESULT_LIST_H_

typedef enum ResultType {
	CONSTANT, REGISTER, VARIABLE, UNSET
} ResType;

// A struct to store the outcome of an individual
// expression parse
typedef struct Result {
	ResType type;
	int result;// This could be an immediate value, an	
             //offset of a register, or an address of a
	           //variable
} Res;

typedef struct ResultListNode {
	Res *data;
	struct ResultListNode *next;
} ResListNode;

typedef struct ResultList {
	ResListNode *head;
} ResList;

Res *
init_res ();

ResListNode *
init_res_list_node ();

ResList *
init_res_list ();

Res *
build_res (ResType t,
					 int val);

ResListNode *
build_res_list_node (Res *data);

void
next_res_list_node (ResListNode **rln);

void
push_res_list_node (ResList **rl,
										ResListNode *new_node);

void
push_res_list_data (ResList **rl,
										Res* new_data);

ResList *
deep_copy_res_list (ResList *src_rl);//calloc

void
print_res_list (ResList *rl);

void
free_res_list (ResList **rl);

#endif//_RESULT_LIST_H_
