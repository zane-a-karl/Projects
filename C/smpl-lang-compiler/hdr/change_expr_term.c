struct AstNode *
smpl_expression (struct Parser *p,
								 Agraph_t      *g,
								 Agnode_t *parent)
{
	struct AstNode *ambig_node = smpl_term(p, g, parent);
	struct AstNode *poss_node;
	if ( peek_tkn(PLUS, p) || peek_tkn(MINUS, p) ) {
		poss_node = new_ast_node(g, parent, "");
		poss_node->bin_op = new_bin_op();
		poss_node->bin_op->lhs = ambig_node;
	}
	while ( peek_tkn(PLUS, p) || peek_tkn(MINUS, p) ) {

		poss_node->bin_op->op = peek_tkn(PLUS, p) ? "+" : "-";
		consume_tkn(p->curr_tkn->tkn->type, p, EXPRESSION);
		poss_node->bin_op->rhs = smpl_term(p, g, poss_node->self);
		if ( !peek_tkn(PLUS, p) ) {
			return poss_node;
		} else {
			poss_node->bin_op->lhs = poss_node;
		}
	}

	return ambig_node;
}

// term = factor {("*" | "/") factor}
struct AstNode *
smpl_term (struct Parser *p,
					 Agraph_t      *g,
					 Agnode_t *parent)
{
	struct AstNode *ambig_node = smpl_factor(p, g, parent);
	struct AstNode *poss_node;
	if ( peek_tkn(ASTERISK, p) || peek_tkn(SLASH, p) ) {

		poss_node = new_ast_node(g, parent, "");
		poss_node->bin_op = new_bin_op();
		poss_node->bin_op->lhs = ambig_node;
	}
	while ( peek_tkn(ASTERISK, p) || peek_tkn(SLASH, p) ) {

		poss_node->bin_op->op = peek_tkn(ASTERISK, p) ? "*" : "/";
		consume_tkn(p->curr_tkn->tkn->type, p, EXPRESSION);
		poss_node->bin_op->rhs =
			smpl_factor(p, g, poss_node->self);
		if ( !peek_tkn(ASTERISK, p) ) {
			return poss_node;
		} else {
			poss_node->bin_op->lhs = poss_node;
		}
	}

	return ambig_node;
}
