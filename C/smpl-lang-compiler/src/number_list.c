#include "../hdr/number_list.h"

struct Number *
new_num ()
{
	struct Number *n = calloc(1, sizeof(struct Number));
	n->val           = 0;
	return n;
}

struct NumberNode *
new_num_node (struct Number *num)
{
	struct NumberNode *nn = calloc(1, sizeof(struct NumberNode));
	nn->data              = num;
	nn->next              = NULL;
	return nn;
}

struct NumberList *
new_num_list ()
{
	struct NumberList *nl = calloc(1, sizeof(struct NumberList));
	nl->head              = NULL;
	return nl;
}

/* void */
/* push_num_list (struct NumberList **nl, */
/* 							 struct NumberList *new_nl) { */

/* 	struct NumberNode *i = (*nl)->head; */
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
push_num_node (struct NumberList *nl,
							 struct NumberNode *new_node)
{
	struct NumberNode *i = nl->head;
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
push_num (struct NumberList *nl,
					struct Number *data)
{
	push_num_node(nl, new_num_node(data));
}

void
print_num_list (struct NumberList *nl)
{
	int idx = 0;
  for (struct NumberNode *i = nl->head;
			 i != NULL;
			 i = i->next) {

    printf("node %d: data=%i\n", idx++, i->data->val);
  }
}

void
free_num_list (struct NumberList **nl)
{
	struct NumberNode *cur = (*nl)->head;
	struct NumberNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free(prv);
	}
}
