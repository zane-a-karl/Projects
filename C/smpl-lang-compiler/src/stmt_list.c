#include "../hdr/stmt_list.h"

struct Stmt *
new_stmt ()
{
	struct Stmt *s = calloc(1, sizeof(struct Stmt));
	s->assignment  = NULL;
	s->func_call   = NULL;
	s->if_stmt     = NULL;
	s->while_stmt  = NULL;
	s->return_stmt = NULL;
	s->next        = NULL;
	return s;
}

struct StmtNode *
new_sn (struct Stmt *stmt)
{
	struct StmtNode *sn = calloc(1, sizeof(struct StmtNode));
	sn->data            = stmt;
	sn->next            = NULL;
	return sn;
}

struct StmtList *
new_sl ()
{
	struct StmtList *sl = calloc(1, sizeof(struct StmtList));
	sl->head            = NULL;
	return sl;
}

// assume `new_node` already calloc'd
void
push_sn (struct StmtList *sl,
				 struct StmtNode *new_node)
{
	struct StmtNode *i = sl->head;
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
push_stmt (struct StmtList *sl,
					 struct Stmt *new_data)
{
	push_sn(sl, new_sn(new_data));
}

void
print_sl (struct StmtList *sl)
{
	//	int idx = 0;
  for (struct StmtNode *i = sl->head;
			 i != NULL;
			 i = i->next) {

		/* printf("dimens[%i]:", idx); */
		/* print_int_list(i->dimens_list); */
		/* printf("idents[%i]:", idx); */
		/* print_str_list(i->idents_list); */
  }
}

void
free_sl (struct StmtList **sl)
{
	struct StmtNode *cur = (*sl)->head;
	struct StmtNode *prv;
  while ( cur != NULL ) {
		prv = cur;
		cur=cur->next;
		/* free_int_list(prv->dimens_list); */
		/* free_str_list(prv->idents_list); */
		free(prv);
	}
}
