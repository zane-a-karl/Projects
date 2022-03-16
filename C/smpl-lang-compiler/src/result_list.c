#include "../hdr/result_list.h"

void
set_result (Result *r,
						ResultType t,
						TokenNode *tn) {

	r->type = t;
	switch (t) {
	case CONSTANT:
		r->data = tn->tkn->val;
		break;
	case REGISTER:// This should be the same as VARIABLE
	case VARIABLE:
		r->data = lookup(tn->tkn->raw_tkn);
		break;
	default:
		perror("Infeasible result");
		exit(1);
		break;
	}
}

ResultList*
init_result_list () {

	ResultList *rl = calloc(1, sizeof(ResultList));
	rl->head = NULL;
	return rl;
}

ResultListNode *
init_result_list_node (Result *data) {

	ResultListNode *rln;
	rln = calloc(1, sizeof(ResultListNode));
	rln->data = data;
	rln->next = NULL;
	return rln;
}

void
next_result_list_node (ResultListNode **rln) {

	(*rln) = (*rln)->next;
}

void
push_result_list (ResultList **rl,
									ResultList *new_rl) {

	ResultListNode *i = (*rl)->head;
	while (i->next != NULL) {
		i = i->next;
	}
	i = new_rl->head;
	while (i->next != NULL) {
		push_result_list_data(rl, i->data);
		i = i->next;
	}
}

// assume new_node already calloc'd
void
push_result_list_node (ResultList **rl,
											 ResultListNode *new_node) {
	
	ResultListNode *i = (*rl)->head;
	if ( i == NULL ) {
		(*rl)->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

void
push_result_list_data (ResultList **rl,
											 Result new_data) {
	
	ResultListNode *new_node =
		init_result_list_node(new_data);//calloc
	push_result_list_node(rl, new_node);
}

ResultList *
deep_copy_result_list (ResultList *src_rl) {//calloc

	ResultList *dst_rl = init_result_list();
	for (ResultListNode *i = src_rl->head;
			 i != NULL;
			 i = i->next) {
		push_result_list_data(&dst_rl, i->data);
	}
	return dst_rl;
}

void
print_result_list (ResultList *rl) {

	result idx = 0;
  for (ResultListNode *i = rl->head;
			 i != NULL;
			 i = i->next) {

    printf("node %d: data=%i\n", idx++, i->data->result);
  }
}

void
free_result_list (ResultList **rl) {

	ResultListNode *cur = (*rl)->head;
	ResultListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur = cur->next;
		free(prv->data);
		free(prv);
	}
}
