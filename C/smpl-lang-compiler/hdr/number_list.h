#ifndef _NUMBER_LIST_H_
#define _NUMBER_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Number {
	int val;
};

struct NumberNode {
	struct Number *data;
	struct NumberNode *next;
};

struct NumberList {
	struct NumberNode *head;
};

struct Number *
new_num ();

struct NumberNode *
new_num_node ();

struct NumberList *
new_num_list ();

/* void */
/* push_num_list (struct NumberList *nl, */
/* 							 struct NumberList *new_nl); */

void
push_num_node (struct NumberList *nl,
							 struct NumberNode *new_node);

void
push_num (struct NumberList *nl,
					struct Number *data);

void
print_num_list (struct NumberList *nl);

void
free_num_list (struct NumberList **nl);

#endif//_NUMBER_LIST_H_
