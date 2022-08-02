#include "../hdr/identifier_list.h"

struct Ident *
new_ident ()
{
	struct Ident *ident = calloc(1, sizeof(struct Ident));
	ident->name         = calloc(MAX_VAR_NAME_LEN, sizeof(char));
	return ident;
}

struct IdentNode *
new_ident_node (struct Ident *ident)
{
	struct IdentNode *in = calloc(1, sizeof(struct IdentNode));
	in->data             = ident;
	in->next             = NULL;
	return in;
}

struct IdentList *
new_ident_list ()
{
	struct IdentList *il = calloc(1, sizeof(struct IdentList));
	il->head             = NULL;
	return il;
}

// assume new_node already calloc'd
void
push_ident_node (struct IdentList *il,
								 struct IdentNode *new_node)
{
	struct IdentNode *i = il->head;
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
push_ident (struct IdentList *il,
						struct Ident *data)
{
	push_ident_node(il, new_ident_node(data));
}

void
print_ident_list (struct IdentList *il)
{
	//	int idx = 0;
  for (struct IdentNode *i = il->head;
			 i != NULL;
			 i = i->next) {
		//    printf("node %d: data=\"%s\"\n", idx++, i->data);
  }
}

void
free_ident_list (struct IdentList **il)
{
	struct IdentNode *cur = (*il)->head;
	struct IdentNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free(prv->data);
		free(prv);
	}
}
