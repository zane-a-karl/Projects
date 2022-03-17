#ifndef _FUNC_DECL_LIST_H_
#define _FUNC_DECL_LIST_H_

#include "../hdr/token.h"
#include "../hdr/identifier_list.h"
#include "../hdr/var_decl_list.h"
#include "../hdr/stmt_list.h"

#include <stdbool.h>
#include <stdlib.h>

#ifndef MAX_VAR_NAME_LEN
#define MAX_VAR_NAME_LEN 1<<4 //16
#endif//MAX_VAR_NAME_LEN

typedef struct FuncDecl {
	Identifier *ident;
	IdentifierList *params;
	VarDeclList *local_vars;
	StmtList *stmts;
	bool is_void;
} Func_Decl;

// Wrapper so we can return local_vars and stmts from
// smpl_func_body
typedef struct FuncBody {
	VarDeclList *local_vars;
	StmtList *stmts;
} FuncBody;

typedef struct FuncDeclListNode {
	FuncDecl *data;
	struct FuncDeclListNode *next;
} FuncDeclListNode;

typedef struct FuncDeclList {
	FuncDeclListNode *head;
} FuncDeclList;

FuncDecl *
init_fd ();

FuncBody *
init_func_body ();

FuncDeclList *
init_fdl ();

FuncDeclListNode *
build_fdln (FuncDecl *data);

void
next_fdln (FuncDeclListNode **fdln);

void
push_fdln (FuncDeclList **fdl,
													FuncDeclListNode *new_node);

void
push_fdl_data (FuncDeclList **fdl,
													FuncDecl new_data);

void
print_fdl (FuncDeclList *fdl);

void
free_fdl (FuncDeclList **fdl);

#endif//_FUNC_DECL_LIST_H_
