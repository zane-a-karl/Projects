#ifndef _IDENTIFIER_H_
#define _IDENTIFIER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_VAR_NAME_LEN
#define MAX_VAR_NAME_LEN 1<<4 //16
#endif//MAX_VAR_NAME_LEN

struct Ident {
	char *name;
};

struct Ident *
new_ident ();

struct Ident *
deep_copy_ident (struct Ident *src);

#endif//_IDENTIFIER_H_
