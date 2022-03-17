#ifndef _IDENTIFIER_LIST_H_
#define _IDENTIFIER_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_VAR_NAME_LEN
#define MAX_VAR_NAME_LEN 1<<4 //16
#endif//MAX_VAR_NAME_LEN

typedef struct Identifier {
	char *name;
} Ident;

typedef struct IdentifierListNode {
	Ident *data;
	struct IdentListNode *next;
} IdentListNode;

typedef struct IdentifierList {
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
build_ident_list_node (void *data);

void
next_ident_list_node (IdentListNode **sln);

void
push_ident_list (IdentList **sl,
								 IdentList *new_sl);

void
push_ident_list_node (IdentList **sl,
											IdentListNode *new_node);

void
push_ident_list_data (IdentList **sl,
											char *new_data);

void
print_ident_list (IdentList *sl);

IdentList *
deep_copy_ident_list (IdentList *src_sl);

void
free_ident_list (IdentList **sl);

#endif//_IDENTIFIER_LIST_H_
