#ifndef _ECB_H_
#define _ECB_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h> // memset()


void
ecb_bools (bool **output, // O
	   bool *input,
	   bool *key,
	   int block_len,
	   int input_len,
	   int key_len);

void
ecb_chars (char **output, // O
	   char *input,
	   char *key,
	   int block_len,
	   int input_len,
	   int key_len);

#endif // _ECB_H_
