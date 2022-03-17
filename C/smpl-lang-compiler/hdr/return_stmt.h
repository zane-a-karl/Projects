#ifndef _RETURN_STMT_H_
#define _RETURN_STMT_H_

#include "../hdr/result_list.h"

typedef struct ReturnStmt {
	union Result *ret_val;
} ReturnStmt;

ReturnStmt *
init_return_stmt ();

#endif//_RETURN_STMT_H_
