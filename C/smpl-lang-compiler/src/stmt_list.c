#include "../hdr/stmt_list.h"

Stmt *
init_stmt () {

	Stmt *s        = calloc(1, sizeof(Stmt));
	s->assignment  = init_assignment();
	s->func_call   = init_func_call();
	s->if_stmt     = init_if_stmt();
	s->while_stmt  = init_while_stmt();
	s->return_stmt = init_return_stmt();
	s->next        = NULL;
	return s;
}

StmtListNode *
init_sln () {

	StmtListNode *sln = calloc(1, sizeof(StmtListNode));
	sln->data         = init_stmt();
	sln->next         = NULL;
	return sln;
}

StmtList *
init_sl () {

	StmtList *sl;
	sl = calloc(1, sizeof(StmtList));
	sl->head = NULL;
	return sl;
}

// assume data already calloc'd
StmtListNode *
build_sln (Stmt *data) {

	StmtListNode *sln = calloc(1, sizeof(StmtListNode));
	sln->data         = data;
	sln->next         = NULL;
	return sln;
}

void
next_sln (StmtListNode **sln) {

	(*sln) = (*sln)->next;
}

// assume new_node already calloc'd
void
push_sln (StmtList **sl,
					StmtListNode *new_node) {

	StmtListNode *i = (*sl)->head;
	if ( i == NULL ) {
		(*sl)->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

// assume new_data already calloc'd
void
push_sl_data (StmtList **sl,
							Stmt *new_data) {
	
	StmtListNode *new_node = build_sln(new_data);
	push_sln(sl, new_node);
}

void
print_sl (StmtList *sl) {

	//	int idx = 0;
  for (StmtListNode *i = sl->head;
			 i != NULL;
			 i = i->next) {

		/* printf("dimens[%i]:", idx); */
		/* print_int_list(i->dimens_list); */
		/* printf("idents[%i]:", idx); */
		/* print_str_list(i->idents_list); */
  }
}

void
free_sl (StmtList **sl) {

	StmtListNode *cur = (*sl)->head;
	StmtListNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		/* free_int_list(prv->dimens_list); */
		/* free_str_list(prv->idents_list); */
		free(prv);
	}
}
