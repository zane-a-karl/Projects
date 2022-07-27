#ifndef _FUNC_DECL_LIST_H_
#define _FUNC_DECL_LIST_H_

typedef struct FuncDecl FuncDecl;

#include "../hdr/token.h"
#include "../hdr/identifier_list.h"
#include "../hdr/var_decl_list.h"
#include "../hdr/stmt_list.h"

#include <stdbool.h>
#include <stdlib.h>

#ifndef MAX_VAR_NAME_LEN
#define MAX_VAR_NAME_LEN 1<<4 //16
#endif//MAX_VAR_NAME_LEN

// Wrapper so we can return local_vars and stmts from
// smpl_func_body
typedef struct FuncBody {
	VarDeclList *local_vars;
	struct StmtList *stmts;
} FuncBody;

struct FuncDecl {
	bool is_void;
	struct Ident *name;
	struct IdentList *params;
	FuncBody *body;
};

typedef struct FuncDeclNode {
	FuncDecl *data;
	struct FuncDeclNode *next;
} FuncDeclNode;

typedef struct FuncDeclList {
	FuncDeclNode *head;
} FuncDeclList;

FuncDecl *
new_fd ();

FuncBody *
new_func_body ();

FuncDeclNode *
new_fdn (FuncDecl *fd);

FuncDeclList *
new_fdl ();

void
push_fdn (FuncDeclList *fdl,
					FuncDeclNode *new_node);

void
push_fd (FuncDeclList *fdl,
				 FuncDecl *data);

void
print_fdl (FuncDeclList *fdl);

void
free_fdl (FuncDeclList **fdl);

#endif//_FUNC_DECL_LIST_H_
