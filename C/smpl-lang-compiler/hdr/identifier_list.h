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

typedef struct IdentNode {
	Ident *data;
	struct IdentNode *next;
} IdentNode;

typedef struct IdentList {
	IdentNode *head;
} IdentList;

Ident *
new_ident ();

IdentNode *
new_ident_node (Ident *ident);

IdentList *
new_ident_list ();

void
push_ident (IdentList *il,
						Ident *data);

void
print_ident_list (IdentList *il);

void
free_ident_list (IdentList **il);

#endif//_IDENTIFIER_LIST_H_
