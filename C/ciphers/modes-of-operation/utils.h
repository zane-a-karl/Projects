#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>
#include <string.h>

void
ascii_to_bits (bool **bit_str, // O
	       char *ascii_str,
	       int bits_per_char);

void
bits_to_ascii (char **ascii_str, // O
	       bool *bit_str,
	       int bit_str_len,
	       int bits_per_char);

#endif // _UTILS_H_
