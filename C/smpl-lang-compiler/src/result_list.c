#include "../hdr/result_list.h"


struct Result *
new_res ()
{
	struct Result *r = calloc(1, sizeof(struct Result));
	r->des           = NULL;
	r->num           = NULL;
	r->bin_op        = NULL;
	r->func_call     = NULL;
	return r;
}

struct ResultNode *
new_res_node (struct Result *result)
{
	struct ResultNode *rn = calloc(1, sizeof(struct ResultNode));
	rn->data              = result;
	rn->next              = NULL;
	return rn;
}

struct ResultList*
new_res_list ()
{
	struct ResultList *rl = calloc(1, sizeof(struct ResultList));
	rl->head              = NULL;
	return rl;
}

// assume `new_node` already calloc'd
void
push_res_node (struct ResultList *rl,
							 struct ResultNode *new_node)
{
	struct ResultNode *i = rl->head;
	if ( i == NULL ) {
		rl->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

// assume `new_data` already calloc'd
void
push_result (struct ResultList *rl,
						 struct Result *new_data)
{
	push_res_node(rl, new_res_node(new_data));
}

void
print_res_list (struct ResultList *rl)
{
	//	int idx = 0;
  for (struct ResultNode *i = rl->head;
			 i != NULL;
			 i = i->next) {

    //printf("node %d: data=%i\n", idx++, i->data->res);
  }
}

void
free_res_list (struct ResultList **rl)
{
	struct ResultNode *cur = (*rl)->head;
	struct ResultNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur = cur->next;
		free(prv->data);
		free(prv);
	}
}
