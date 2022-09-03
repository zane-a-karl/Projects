#include "../hdr/str_list.h"

//Assume that `data` is already calloc'd
struct StrNode *
new_str_node (char *data)
{
	struct StrNode *sn = calloc(1, sizeof(struct StrNode));
	sn->data           = data;
	sn->next           = NULL;

	return sn;
}

struct StrList *
new_str_list ()
{
	struct StrList *sl = calloc(1, sizeof(struct StrList));
	sl->head           = NULL;

	return sl;
}

// assume `new_data` already calloc'd
void
push_str (struct StrList *sl,
					char           *new_data)
{
	struct StrNode *new_node = new_str_node(new_data);
	struct StrNode *i = sl->head;
	if ( i == NULL ) {
		sl->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

void
free_str_list (struct StrList **sl)
{
	struct StrNode *cur = (*sl)->head;
	struct StrNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free(prv->data);
		free(prv);
	}
}
