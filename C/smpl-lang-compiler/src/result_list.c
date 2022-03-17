#include "../hdr/result_list.h"


Res *
init_res () {

	Res *r    = calloc(1, sizeof(Res));
	r->type   = UNSET;
	r->result = 0;
	return r;
}

ResListNode *
init_res_list_node () {

	ResListNode *rln = calloc(1, sizeof(ResListNode));
	rln->data        = NULL;
	rln->next        = NULL;
	return rln;
}

ResList*
init_res_list () {

	ResList *rl = calloc(1, sizeof(ResList));
	rl->head    = NULL;
	return rl;
}

Res *
build_res (ResType t,
					 int val) {

	r->type = t;
	switch (t) {
	case CONSTANT:
		r->data = val;
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

// assume data already calloc'd
ResListNode *
build_res_list_node (Res *data) {

	ResListNode *rln;
	rln = calloc(1, sizeof(ResListNode));
	rln->data = data;
	rln->next = NULL;
	return rln;
}

void
next_res_list_node (ResListNode **rln) {

	(*rln) = (*rln)->next;
}

// assume new_node already calloc'd
void
push_res_list_node (ResList **rl,
										ResListNode *new_node) {
	
	ResListNode *i = (*rl)->head;
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
push_res_list_data (ResList **rl,
										Res *new_data) {
	
	ResListNode *new_node = build_res_list_node(new_data);
	push_res_list_node(rl, new_node);
}

ResList *
deep_copy_res_list (ResList *src_rl) {//calloc

	ResList *dst_rl = init_res_list();
	for (ResListNode *i = src_rl->head;
			 i != NULL;
			 i = i->next) {
		push_res_list_data(&dst_rl, i->data);
	}
	return dst_rl;
}

void
print_res_list (ResList *rl) {

	res idx = 0;
  for (ResListNode *i = rl->head;
			 i != NULL;
			 i = i->next) {

    printf("node %d: data=%i\n", idx++, i->data->res);
  }
}

void
free_res_list (ResList **rl) {

	ResListNode *cur = (*rl)->head;
	ResListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur = cur->next;
		free(prv->data);
		free(prv);
	}
}
