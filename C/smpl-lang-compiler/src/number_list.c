#include "../hdr/number_list.h"

Num *
init_num () {

	Num *n = calloc(1, sizeof(Num));
	n->val = 0;
	return n;
}

NumListNode *
init_num_list_node () {

	NumListNode *nln;
	nln       = calloc(1, sizeof(NumListNode));
	nln->data = init_num();
	nln->next = NULL;
	return nln;
}

NumList *
init_num_list () {

	NumList *nl = calloc(1, sizeof(NumList));
	nl->head = NULL;
	return nl;
}

Num *
build_num (int val) {

	Num *n = calloc(1, sizeof(Num));
	n->val = val;
	return n;
}

// assume data already calloc'd
NumListNode *
build_num_list_node (Num *data) {

	NumListNode *nln;
	nln       = calloc(1, sizeof(NumListNode));
	nln->data = data;
	nln->next = NULL;
	return nln;
}

void
next_num_list_node (NumListNode **nln) {

	(*nln) = (*nln)->next;
}

void
push_num_list (NumList **nl,
							 NumList *new_nl) {

	NumListNode *i = (*nl)->head;
	while (i->next != NULL) {
		i = i->next;
	}
	i = new_nl->head;
	while (i->next != NULL) {
		push_num_list_data(nl, i->data);
		i = i->next;
	}
}

// assume new_node already calloc'd
void
push_num_list_node (NumList **nl,
										NumListNode *new_node) {
	
	NumListNode *i = (*nl)->head;
	if ( i == NULL ) {
		(*nl)->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

void
push_num_list_data (NumList **nl,
										Num *new_data) {

	// New nln calloc'd here
	NumListNode *new_node = build_num_list_node(new_data);
	push_num_list_node(nl, new_node);
}

NumList *
deep_copy_num_list (NumList *src_nl) {//calloc

	NumList *dst_nl = init_num_list();
	for (NumListNode *i = src_nl->head;
			 i != NULL;
			 i = i->next) {
		
		push_num_list_data(&dst_nl, i->data);
	}
	return dst_nl;
}

void
print_num_list (NumList *nl) {

	int idx = 0;
  for (NumListNode *i = nl->head;
			 i != NULL;
			 i = i->next) {

    printf("node %d: data=%i\n", idx++, i->data->val);
  }
}

void
free_num_list (NumList **nl) {

	NumListNode *cur = (*nl)->head;
	NumListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free(prv);
	}
}
