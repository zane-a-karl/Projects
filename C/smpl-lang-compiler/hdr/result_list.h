#ifndef _RESULT_LIST_H_
#define _RESULT_LIST_H_

// Do this to silence circular dependency
typedef struct Result Res;

#include "../hdr/number_list.h"
#include "../hdr/bin_op.h"
#include "../hdr/designator.h"
#include "../hdr/func_call.h"

typedef struct Result {
	union {
		Designator *des;
		Num *num;
		BinOp *bin_op;
		struct FuncCall *func_call;
	};
} Result;

typedef struct ResultNode {
	Res *data;
	struct ResultNode *next;
} ResNode;

typedef struct ResultList {
	ResNode *head;
} ResList;

Res *
new_res ();

ResNode *
new_res_node (Res *result);

ResList *
new_res_list ();

void
push_res_node (ResList *rl,
							 ResNode *new_node);

void
push_result (ResList *rl,
						 Res *new_data);

void
print_res_list (ResList *rl);

void
free_res_list (ResList **rl);

#endif//_RESULT_LIST_H_
