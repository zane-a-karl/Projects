#ifndef _NUMBER_LIST_H_
#define _NUMBER_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Number {
	int val;
} Num;

typedef struct NumberListNode {
	Num *data;
	struct NumberListNode *next;
} NumListNode;

typedef struct NumberList {
	NumListNode *head;
} NumList;

Num *
init_num ();

NumListNode *
init_num_list_node ();

NumList *
init_num_list ();

Num *
build_num (int val);

NumListNode *
build_num_list_node (Num *num);

void
next_num_list_node (NumListNode **nln);

void
push_num_list (NumList **nl,
							 NumList *new_nl);

void
push_num_list_node (NumList **nl,
										NumListNode *new_node);

void
push_num_list_data (NumList **nl,
										Num *new_data);

NumList *
deep_copy_num_list (NumList *src_nl);//calloc

void
print_num_list (NumList *nl);

void
free_num_list (NumList **nl);

#endif//_NUMBER_LIST_H_
