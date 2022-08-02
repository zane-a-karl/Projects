#ifndef _IDENTIFIER_LIST_H_
#define _IDENTIFIER_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_VAR_NAME_LEN
#define MAX_VAR_NAME_LEN 1<<4 //16
#endif//MAX_VAR_NAME_LEN

struct Ident {
	char *name;
};

struct IdentNode {
	struct Ident *data;
	struct IdentNode *next;
};

struct IdentList {
	struct IdentNode *head;
};

struct Ident *
new_ident ();

struct IdentNode *
new_ident_node (struct Ident *ident);

struct IdentList *
new_ident_list ();

void
push_ident (struct IdentList *il,
						struct Ident *data);

void
print_ident_list (struct IdentList *il);

void
free_ident_list (struct IdentList **il);

#endif//_IDENTIFIER_LIST_H_
