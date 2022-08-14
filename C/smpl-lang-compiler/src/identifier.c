#include "../hdr/identifier.h"

struct Ident *
new_ident ()
{
	struct Ident *ident = calloc(1, sizeof(struct Ident));
	ident->name         = calloc(MAX_VAR_NAME_LEN,
															 sizeof(char));
	return ident;
}

struct Ident *
deep_copy_ident (struct Ident *src)
{
	struct Ident *dst = new_ident();
	for (int i = 0; i < MAX_VAR_NAME_LEN; ++i) {
		dst->name[i] =	src->name[i];
	}
	return dst;
}
