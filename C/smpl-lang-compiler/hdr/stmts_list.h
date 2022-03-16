#ifndef _STMTS_LIST_H_
#define _STMTS_LIST_H_

#include "../hdr/token.h"
#include "../hdr/int_list.h"
#include "../hdr/str_list.h"
#include "../hdr/designator.h"
#include "../hdr/result_list.h"

#include <stdlib.h>

typedef enum StmtType {
	LET, CALL, IF, WHILE, RETURN, UNSET;
} StmtType;
// TODO: think about a better way than circularly
// dependent types like this
typedef struct StmtsListNode {
	StmtType type;
	Designator *lhs;                   //LET
	Result *rhs;                       //LET
	char *fn_name;                     //CALL
	struct StmtsList *arg_expressions; //CALL
	char *condition;                   //IF/WH
	struct StmtsList *stmts_then_do;   //IF/WH
	struct StmtsList *stmts_else;      //IF
	int value;                         //RET
	struct StmtsListNode *next;
} StmtsListNode;

typedef struct StmtsList {
	struct StmtsListNode *head;
} StmtsList;

StmtsList *
init_stmts_list ();

StmtsListNode *
init_stmts_list_node ();

void
next_stmts_list_node (StmtsListNode **sln);

void
push_stmts_list_node (StmtsList **sl,
										 StmtsListNode *new_node);

void
print_stmts_list (StmtsList *sl);

void
free_stmts_list (StmtsList **sl);

#endif//_STMTS_LIST_H_
