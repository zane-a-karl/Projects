#include "../hdr/result_list.h"


Res *
new_res ()
{
	Res *r       = calloc(1, sizeof(Res));
	r->des       = NULL;
	r->num       = NULL;
	r->bin_op    = NULL;
	r->func_call = NULL;
	return r;
}

ResNode *
new_res_node (Res *result)
{
	ResNode *rn = calloc(1, sizeof(ResNode));
	rn->data    = result;
	rn->next    = NULL;
	return rn;
}

ResList*
new_res_list ()
{
	ResList *rl = calloc(1, sizeof(ResList));
	rl->head    = NULL;
	return rl;
}

// assume `new_node` already calloc'd
void
push_res_node (ResList *rl,
							 ResNode *new_node)
{
	ResNode *i = rl->head;
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
push_result (ResList *rl,
						 Res *new_data)
{
	push_res_node(rl, new_res_node(new_data));
}

void
print_res_list (ResList *rl)
{
	//	int idx = 0;
  for (ResNode *i = rl->head;
			 i != NULL;
			 i = i->next) {

    //printf("node %d: data=%i\n", idx++, i->data->res);
  }
}

void
free_res_list (ResList **rl)
{
	ResNode *cur = (*rl)->head;
	ResNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur = cur->next;
		free(prv->data);
		free(prv);
	}
}
