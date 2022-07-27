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
	struct StmtNode *next;
} Stmt;

// TODO: think about a better way than circularly
// dependent types like this
typedef struct StmtNode {
	Stmt *data;
	struct StmtNode *next;
} StmtNode;

typedef struct StmtList {
	struct StmtNode *head;
} StmtList;

Stmt *
new_stmt ();

StmtNode *
new_sn (Stmt *stmt);

StmtList *
new_sl ();

void
push_sn (StmtList *sl,
				 StmtNode *new_node);

void
push_stmt (StmtList *sl,
					 Stmt *data);

void
print_sl (StmtList *sl);

void
free_sl (StmtList **sl);

#endif//_STMT_LIST_H_
