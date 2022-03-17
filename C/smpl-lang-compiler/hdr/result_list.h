#ifndef _RESULT_LIST_H_
#define _RESULT_LIST_H_

// Do this to silence circular dependency
typedef union Result Res;

#include "../hdr/number_list.h"
#include "../hdr/bin_op.h"
#include "../hdr/designator.h"
#include "../hdr/func_call.h"

union Result {
	Designator *des;
	Num *num;
	BinOp *bin_op;
	struct FuncCall *func_call;
};

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

void
print_res_list (ResList *rl);

void
free_res_list (ResList **rl);

#endif//_RESULT_LIST_H_
