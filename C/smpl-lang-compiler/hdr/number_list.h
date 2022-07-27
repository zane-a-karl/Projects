#ifndef _NUMBER_LIST_H_
#define _NUMBER_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Number {
	int val;
} Num;

typedef struct NumberNode {
	Num *data;
	struct NumberNode *next;
} NumNode;

typedef struct NumberList {
	NumNode *head;
} NumList;

Num *
new_num ();

NumNode *
new_num_node ();

NumList *
new_num_list ();

/* void */
/* push_num_list (NumList *nl, */
/* 							 NumList *new_nl); */

void
push_num_node (NumList *nl,
							 NumNode *new_node);

void
push_num (NumList *nl,
					Num *data);

void
print_num_list (NumList *nl);

void
free_num_list (NumList **nl);

#endif//_NUMBER_LIST_H_
