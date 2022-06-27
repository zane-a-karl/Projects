#ifndef _IDENTIFIER_LIST_H_
#define _IDENTIFIER_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_VAR_NAME_LEN
#define MAX_VAR_NAME_LEN 1<<4 //16
#endif//MAX_VAR_NAME_LEN

typedef struct Ident {
	char *name;
} Ident;

typedef struct IdentListNode {
	Ident *data;
	struct IdentListNode *next;
} IdentListNode;

typedef struct IdentList {
	IdentListNode *head;
} IdentList;

Ident *
init_ident ();

IdentListNode *
init_ident_list_node ();

IdentList *
init_ident_list ();

Ident *
build_ident (char *name);

IdentListNode *
build_ident_list_node (Ident *data);

void
next_ident_list_node (IdentListNode **iln);

void
push_ident_list (IdentList **il,
								 IdentList *new_il);

void
push_ident_list_node (IdentList **il,
											IdentListNode *new_node);

void
push_ident_list_data (IdentList **il,
											Ident *new_data);

void
print_ident_list (IdentList *il);

void
free_ident_list (IdentList **il);

#endif//_IDENTIFIER_LIST_H_
