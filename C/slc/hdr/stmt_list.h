#ifndef _STMT_LIST_H_
#define _STMT_LIST_H_

#include "../hdr/token.h"
#include "../hdr/number_list.h"
#include "../hdr/identifier_list.h"
#include "../hdr/designator.h"
#include "../hdr/result_list.h"
#include "../hdr/assignment.h"
#include "../hdr/if_stmt.h"
#include "../hdr/while_stmt.h"
#include "../hdr/return_stmt.h"

#include <stdlib.h>

typedef struct Stmt {
	Assignment *assignment;
	FuncCall *func_call;
	struct IfStmt *if_stmt;
	struct WhileStmt *while_stmt;
	ReturnStmt *return_stmt;
	struct StmtListNode *next;
} Stmt;

// TODO: think about a better way than circularly
// dependent types like this
typedef struct StmtListNode {
	Stmt *data;
	struct StmtListNode *next;
} StmtListNode;

typedef struct StmtList {
	struct StmtListNode *head;
} StmtList;

Stmt *
init_stmt ();

StmtListNode *
init_sln ();

StmtList *
init_sl ();

StmtListNode *
build_sln ();

void
next_sln (StmtListNode **sln);

void
push_sln (StmtList **sl,
					StmtListNode *new_node);

void
push_sl_data (StmtList **sl,
							Stmt *new_data);

void
print_sl (StmtList *sl);

void
free_sl (StmtList **sl);

#endif//_STMT_LIST_H_
