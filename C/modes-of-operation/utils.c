#include "utils.h"


/** Converts the ascii str into a bit str of length =
 * (strlen * bitwidth) stored in big endian.
 * Assume bit_str has already been calloc'd.
 */
void
ascii_to_bits (bool **bit_str, // O
	       char *ascii_str) {

  int bits_per_char = 8;
  int len = strlen(ascii_str);
  for (int i = 0; i < len; ++i) {
    for (int j = 0; j < bits_per_char; ++j) {
      (*bit_str)[bits_per_char*i + j] =
	ascii_str[i] & (1<<(bits_per_char-j-1));
    }
  }
}

/** Converts the bit str into an ascii str of
 * length = bit_str_len / bits_per_char stored in big endian.
 * Assume ascii_str has already been calloc'd.
 */
void
bits_to_ascii (char **ascii_str, // O
	       bool *bit_str,
	       int bit_str_len) {

  int bits_per_char = 8;
  for (int i = 0; i < bit_str_len; ++i) {
    (*ascii_str)[i/bits_per_char] +=
      bit_str[i] * (1<<(bits_per_char-(i%bits_per_char)-1));
  }
}
