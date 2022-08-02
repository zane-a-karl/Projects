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

struct Stmt {
	struct Assignment *assignment;
	struct FuncCall *func_call;
	struct IfStmt *if_stmt;
	struct WhileStmt *while_stmt;
	struct ReturnStmt *return_stmt;
	struct StmtNode *next;
};

// TODO: think about a better way than circularly
// dependent types like this
struct StmtNode {
	struct Stmt *data;
	struct StmtNode *next;
};

struct StmtList {
	struct StmtNode *head;
};

struct Stmt *
new_stmt ();

struct StmtNode *
new_sn (struct Stmt *stmt);

struct StmtList *
new_sl ();

void
push_sn (struct StmtList *sl,
				 struct StmtNode *new_node);

void
push_stmt (struct StmtList *sl,
					 struct Stmt *data);

void
print_sl (struct StmtList *sl);

void
free_sl (struct StmtList **sl);

#endif//_STMT_LIST_H_
