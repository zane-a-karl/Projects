#include "../hdr/stmt_list.h"

Stmt *
new_stmt ()
{
	Stmt *s        = calloc(1, sizeof(Stmt));
	s->assignment  = NULL;
	s->func_call   = NULL;
	s->if_stmt     = NULL;
	s->while_stmt  = NULL;
	s->return_stmt = NULL;
	s->next        = NULL;
	return s;
}

StmtNode *
new_sn (Stmt *stmt)
{
	StmtNode *sn = calloc(1, sizeof(StmtNode));
	sn->data     = stmt;
	sn->next     = NULL;
	return sn;
}

StmtList *
new_sl ()
{
	StmtList *sl = calloc(1, sizeof(StmtList));
	sl->head     = NULL;
	return sl;
}

// assume `new_node` already calloc'd
void
push_sn (StmtList *sl,
				 StmtNode *new_node)
{
	StmtNode *i = sl->head;
	if ( i == NULL ) {
		sl->head = new_node;
	} else {
		while ( i->next != NULL ) {
			i = i->next;
		}
		i->next = new_node;
	}
}

// assume `new_data` already calloc'd
void
push_stmt (StmtList *sl,
					 Stmt *new_data)
{
	push_sn(sl, new_sn(new_data));
}

void
print_sl (StmtList *sl)
{
	//	int idx = 0;
  for (StmtNode *i = sl->head;
			 i != NULL;
			 i = i->next) {

		/* printf("dimens[%i]:", idx); */
		/* print_int_list(i->dimens_list); */
		/* printf("idents[%i]:", idx); */
		/* print_str_list(i->idents_list); */
  }
}

void
free_sl (StmtList **sl)
{
	StmtNode *cur = (*sl)->head;
	StmtNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		/* free_int_list(prv->dimens_list); */
		/* free_str_list(prv->idents_list); */
		free(prv);
	}
}
