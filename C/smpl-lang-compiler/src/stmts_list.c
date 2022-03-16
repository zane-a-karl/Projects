#include "../hdr/stmts_list.h"

StmtsList *
init_stmts_list () {

	StmtsList *sl;
	sl = calloc(1, sizeof(StmtsList));
	sl->head = NULL;
	return sl;
}

StmtsListNode *
init_stmts_list_node () {

	StmtsListNode *sln;
	sln = calloc(1, sizeof(StmtsListNode));
	sln->type = UNSET;
	sln->lhs             = init_designator();
	sln->rhs             = init_result();
	sln->arg_expressions = init_stmts_list();
	sln->condition       = init_relation();
	sln->stmts_then_do   = init_stmts_list();
	sln->stmts_else      = init_stmts_list();
	sln->value = 0;
	sln->next = NULL;
	return sln;
}

void
next_stmts_list_node (StmtsListNode **sln) {

	(*sln) = (*sln)->next;
}

// assume new_node already calloc'd
void
push_stmts_list_node (StmtsList **sl,
											StmtsListNode *new_node) {

	StmtsListNode *i = (*sl)->head;
	if ( i == NULL ) {
		(*sl)->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

void
print_stmts_list (StmtsList *sl) {

	int idx = 0;
  for (StmtsListNode *i = sl->head;
			 i != NULL;
			 i = i->next) {

		/* printf("dimens[%i]:", idx); */
		/* print_int_list(i->dimens_list); */
		/* printf("idents[%i]:", idx); */
		/* print_str_list(i->idents_list); */
  }
}

void
free_stmts_list (StmtsList **sl) {

	StmtsListNode *cur = (*sl)->head;
	StmtsListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		/* free_int_list(prv->dimens_list); */
		/* free_str_list(prv->idents_list); */
		free(prv);
	}
}
