#ifndef _NUMBER_H_
#define _NUMBER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Number {
	int val;
};

struct Number *
new_number ();

struct Number *
deep_copy_number (struct Number *src);

#endif//_NUMBER_H_
