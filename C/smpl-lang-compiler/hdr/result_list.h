#ifndef _RESULT_LIST_H_
#define _RESULT_LIST_H_

#include "../hdr/number_list.h"
#include "../hdr/bin_op.h"
#include "../hdr/designator.h"
#include "../hdr/func_call.h"

struct Result {
	union {
		struct Designator *des;
		struct Number     *num;
		struct BinOp      *bin_op;
		struct FuncCall   *func_call;
	};
};

struct ResultNode {
	struct Result *data;
	struct ResultNode *next;
};

struct ResultList {
	struct ResultNode *head;
};

struct Result *
new_res ();

struct ResultNode *
new_res_node (struct Result *result);

struct ResultList *
new_res_list ();

void
push_res_node (struct ResultList *rl,
							 struct ResultNode *new_node);

void
push_result (struct ResultList *rl,
						 struct Result *new_data);

void
print_res_list (struct ResultList *rl);

void
free_res_list (struct ResultList **rl);

#endif//_RESULT_LIST_H_
