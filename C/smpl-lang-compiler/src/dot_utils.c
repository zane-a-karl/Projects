#include "../hdr/dot_utils.h"


/**
 * Objective here is to make all of the agedge() connections
 * with the necessary visual attributes that we weren't able
 * to do on the fly during parsing.
 */
void
gen_dot_graph (struct Ast *ast,
							 FILE *fout)
{
	// color init
	agattr(ast->graph, AGRAPH, "color", "black");
	agattr(ast->graph, AGNODE, "color", "black");
	agattr(ast->graph, AGEDGE, "color", "black");

	// label init
	agattr(ast->graph, AGNODE, "label", "default_node");
	agattr(ast->graph, AGEDGE, "label", "default_edge");

	// shape init
	agattr(ast->graph, AGNODE, "shape", "hexagon");
	
	create_agedge_set(ast->root);
	agwrite(ast->graph, fout);
}
