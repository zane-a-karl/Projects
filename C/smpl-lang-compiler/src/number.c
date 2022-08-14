#include "../hdr/number.h"

struct Number *
new_number ()
{
	struct Number *n = calloc(1, sizeof(struct Number));
	n->val           = 0;
	return n;
}

struct Number *
deep_copy_number (struct Number *src)
{
	struct Number *dst = new_number();
	dst->val =	src->val;
	
	return dst;
}
