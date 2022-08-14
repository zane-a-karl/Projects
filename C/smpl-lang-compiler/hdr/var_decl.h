#ifndef _VAR_DECL_H_
#define _VAR_DECL_H_

#include "../hdr/ast.h"

#include <stdlib.h>

struct VarDecl {
	struct AstNode     *ident; //Identifier
	struct AstNodeList *dims;  //Dimensions
};

struct VarDecl *
new_vd ();

struct VarDecl *
deep_copy_var_decl (struct VarDecl *src);

#endif//_VAR_DECL_H_
