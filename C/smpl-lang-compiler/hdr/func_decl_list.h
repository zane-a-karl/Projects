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

// Wrapper so we can return local_vars and stmts from
// smpl_func_body
struct FuncBody {
	struct VarDeclList *local_vars;
	struct StmtList *stmts;
};

struct FuncDecl {
	bool is_void;
	struct Ident *name;
	struct IdentList *params;
	struct FuncBody *body;
};

struct FuncDeclNode {
	struct FuncDecl *data;
	struct FuncDeclNode *next;
};

struct FuncDeclList {
	struct FuncDeclNode *head;
};

struct FuncDecl *
new_fd ();

struct FuncBody *
new_func_body ();

struct FuncDeclNode *
new_fdn (struct FuncDecl *fd);

struct FuncDeclList *
new_fdl ();

void
push_fdn (struct FuncDeclList *fdl,
					struct FuncDeclNode *new_node);

void
push_fd (struct FuncDeclList *fdl,
				 struct FuncDecl *data);

void
print_fdl (struct FuncDeclList *fdl);

void
free_fdl (struct FuncDeclList **fdl);

#endif//_FUNC_DECL_LIST_H_
