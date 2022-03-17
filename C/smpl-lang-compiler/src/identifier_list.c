#include "../hdr/identifier_list.h"

Ident *
init_ident () {

	Ident *id = calloc(1, sizeof(Ident));
	id->name  = calloc(MAX_VAR_NAME_LEN, sizeof(char));
	return id;
}

IdentListNode *
init_ident_list_node () {

	IdentListNode *iln;
	iln       = calloc(1, sizeof(IdentListNode));
	iln->data = calloc(1, sizeof(Ident));
	iln->next = NULL;
	return iln;
}

IdentList *
init_ident_list () {

	IdentList *il = calloc(1, sizeof(IdentList));
	il->head = NULL;
	return il;
}

Ident *
build_ident (char *name) {

	Ident *id = calloc(1, sizeof(Ident));
	id->name  = calloc(MAX_VAR_NAME_LEN, sizeof(char));
	for (int i = 0; i < MAX_VAR_NAME_LEN; ++i) {
		id->name[i] = name[i];
	}
	return id;
}

// assume data already calloc'd
IdentListNode *
build_ident_list_node (Ident *data) {

	IdentListNode *iln;
	iln       = calloc(1, sizeof(IdentListNode));
	iln->data = data;
	iln->next = NULL;
	return iln;
}

void
next_ident_list_node (IdentListNode **iln) {

	(*iln) = (*iln)->next;
}

void
push_ident_list (IdentList **il,
								 IdentList *new_il) {

	IdentListNode *i = (*il)->head;
	while (i->next != NULL) {
		i = i->next;
	}
	i = new_il->head;
	while (i->next != NULL) {
		push_ident_list_data(il, i->data);
		i = i->next;
	}
}

// assume new_node already calloc'd
void
push_ident_list_node (IdentList **il,
											IdentListNode *new_node) {

	IdentListNode *i = (*il)->head;
	if ( i == NULL ) {
		(*il)->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

// assume new_data already calloc'd
void
push_ident_list_data (IdentList **il,
											Ident *new_data) {

	IdentListNode *new_node;
	new_node = build_ident_list_node(new_data);
	push_ident_list_node(il, new_node);
}

void
print_ident_list (IdentList *il) {

	//	int idx = 0;
  for (IdentListNode *i = il->head;
			 i != NULL;
			 i = i->next) {
		//    printf("node %d: data=\"%s\"\n", idx++, i->data);
  }
}

void
free_ident_list (IdentList **il) {

	IdentListNode *cur = (*il)->head;
	IdentListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		free(prv->data);
		free(prv);
	}
}
