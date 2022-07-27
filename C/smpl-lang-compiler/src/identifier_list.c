#include "../hdr/identifier_list.h"

Ident *
new_ident ()
{
	Ident *ident = calloc(1, sizeof(Ident));
	ident->name  = calloc(MAX_VAR_NAME_LEN, sizeof(char));
	return ident;
}

IdentNode *
new_ident_node (Ident *ident)
{
	IdentNode *in = calloc(1, sizeof(IdentNode));
	in->data      = ident;
	in->next      = NULL;
	return in;
}

IdentList *
new_ident_list ()
{
	IdentList *il = calloc(1, sizeof(IdentList));
	il->head      = NULL;
	return il;
}

// assume new_node already calloc'd
void
push_ident_node (IdentList *il,
								 IdentNode *new_node)
{
	IdentNode *i = il->head;
	if ( i == NULL ) {
		il->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

// assume `data` already calloc'd
void
push_ident (IdentList *il,
						Ident *data)
{
	push_ident_node(il, new_ident_node(data));
}

void
print_ident_list (IdentList *il)
{
	//	int idx = 0;
  for (IdentNode *i = il->head;
			 i != NULL;
			 i = i->next) {
		//    printf("node %d: data=\"%s\"\n", idx++, i->data);
  }
}

void
free_ident_list (IdentList **il)
{
	IdentNode *cur = (*il)->head;
	IdentNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free(prv->data);
		free(prv);
	}
}
