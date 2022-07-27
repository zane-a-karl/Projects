#include "../hdr/number_list.h"

Num *
new_num ()
{
	Num *n = calloc(1, sizeof(Num));
	n->val = 0;
	return n;
}

NumNode *
new_num_node (Num *num)
{
	NumNode *nn = calloc(1, sizeof(NumNode));
	nn->data    = num;
	nn->next    = NULL;
	return nn;
}

NumList *
new_num_list ()
{
	NumList *nl = calloc(1, sizeof(NumList));
	nl->head    = NULL;
	return nl;
}

/* void */
/* push_num_list (NumList **nl, */
/* 							 NumList *new_nl) { */

/* 	NumNode *i = (*nl)->head; */
/* 	while (i->next != NULL) { */
/* 		i = i->next; */
/* 	} */
/* 	i = new_nl->head; */
/* 	while (i->next != NULL) { */
/* 		push_num_list_data(nl, i->data); */
/* 		i = i->next; */
/* 	} */
/* } */

// assume new_node already calloc'd
void
push_num_node (NumList *nl,
							 NumNode *new_node)
{
	NumNode *i = nl->head;
	if ( i == NULL ) {
		nl->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

// assume `data` already calloc'd
void
push_num (NumList *nl,
					Num *data)
{
	push_num_node(nl, new_num_node(data));
}

void
print_num_list (NumList *nl)
{
	int idx = 0;
  for (NumNode *i = nl->head;
			 i != NULL;
			 i = i->next) {

    printf("node %d: data=%i\n", idx++, i->data->val);
  }
}

void
free_num_list (NumList **nl)
{
	NumNode *cur = (*nl)->head;
	NumNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free(prv);
	}
}
