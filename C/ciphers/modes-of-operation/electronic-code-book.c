#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>


/** Converts the ascii str into a bit str of length =
 * (strlen * bitwidth) stored in big endian.
 * Assume bit_str has already been calloc'd.
 */
void
ascii_to_bits (bool **bit_str, // O
	       char *ascii_str,
	       int bits_per_char) {

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
	       int bit_str_len,
	       int bits_per_char) {

  int ascii_str_len = bit_str_len / bits_per_char;
  for (int i = 0; i < bit_str_len; ++i) {
    (*ascii_str)[i/bits_per_char] +=
      bit_str[i] * (1<<(bits_per_char-(i%bits_per_char)-1));
  }
}

/**
 * TODO for next time:
 *  - write the ecb function for int arrays ie ptxt, ctxt, and key are int *'s
 *  - take advantage of the parallelization opportuity that ecb creates using
     the pthread library.
 *  - do a shared memory key and then socket io message test with ecb.
 *  - write note comments for ecb to remind you what ecb is (maybe include
     diagrams).
 */

/** Encode and Decode are the same function, just with the
 * inputs moved around.
 * Notice that should the key length > block_len only the first
 * block_len bits of the key will be used. Should key length <
 * block_len then the final (block_len-key_len) bits will be
 * filled by the first (block_len-key_len) bits of the key.
 */
void
ecb_bit_by_bit (bool **output,
		bool *input,
		bool *key,
		int block_len,
		int input_len,
		int key_len) {

  bool *modified_key = (bool *)calloc(block_len,
				      sizeof *modified_key);
  memset(*output, 0, input_len * sizeof **output);
  for (int i = 0; i < block_len; ++i) {
    modified_key[i] = key[ i%key_len ];
  }
  for (int i = 0; i < input_len; ++i) {
    (*output)[i] = input[i] ^ modified_key[ i%block_len ];
  }
  free(modified_key);
}

int
main() {

  char *key = "z@nezan3";
  int key_len = strlen(key);
  printf("%s\n", key);
  char *ptxt = "Attack at dawn!";
  int ptxt_len = strlen(ptxt);
  int bits_per_char = 8;
  int block_width = 8;

  for (int i = 0; i < ptxt_len; ++i) {
    printf("%d", ptxt[i]);
    if (i==ptxt_len-1) {
      printf("\n");
    } else {
      printf("  ");
    }
  }

  bool *ptxt_bits = (bool *)calloc(ptxt_len*bits_per_char,
				   sizeof *ptxt_bits);
  ascii_to_bits(&ptxt_bits, ptxt, bits_per_char);
  bool *key_bits = (bool *)calloc(key_len*bits_per_char,
				  sizeof *key_bits);
  ascii_to_bits(&key_bits, key, bits_per_char);

  printf("##### PTXT_BITS #########\n");
  for (int i = 0; i < ptxt_len; ++i) {
    for (int j = 0; j < bits_per_char; ++j) {
      printf("%d", ptxt_bits[bits_per_char*i + j]);
    }
    printf("\n");
  }
  printf("##### KEY_BITS #########\n");
  for (int i = 0; i < key_len; ++i) {
    for (int j = 0; j < bits_per_char; ++j) {
      printf("%d", key_bits[bits_per_char*i + j]);
    }
    printf("\n");
  }

  printf("##### CTXT_BITS #########\n");
  bool *ctxt_bits = (bool *)calloc(ptxt_len*bits_per_char,
				   sizeof *ctxt_bits);
  ecb_bit_by_bit(&ctxt_bits,
		 ptxt_bits,
		 key_bits,
		 block_width,
		 ptxt_len*bits_per_char,
		 key_len*bits_per_char);
  for (int i = 0; i < ptxt_len; ++i) {
    for (int j = 0; j < bits_per_char; ++j) {
      printf("%d", ctxt_bits[bits_per_char*i + j]);
    }
    printf("\n");
  }

  printf("##### CTXT #########\n");
  char *ctxt = (char *)calloc(ptxt_len, sizeof *ctxt);
  bits_to_ascii(&ctxt,
		ctxt_bits,
		ptxt_len*bits_per_char,
		bits_per_char);
  for (int i = 0; i < ptxt_len; ++i) {
    printf("%d", ctxt[i]);
    if (i==ptxt_len-1) {
      printf("\n");
    } else {
      printf("  ");
    }
  }
  printf("##### RECOVERED PTXT #########\n");
  bool *rcvrd_bits = (bool *)calloc(ptxt_len*bits_per_char,
				    sizeof *rcvrd_bits);
  ecb_bit_by_bit(&rcvrd_bits,
		 ctxt_bits,
		 key_bits,
		 block_width,
		 ptxt_len*bits_per_char,
		 key_len*bits_per_char);
  char *rcvrd = (char *)calloc(ptxt_len, sizeof *rcvrd);
  bits_to_ascii(&rcvrd,
		rcvrd_bits,
		ptxt_len*bits_per_char,
		bits_per_char);
  printf("%s\n", rcvrd);

  //  free(ptxt);
  free(ctxt);
  //  free(key);
  free(rcvrd);
  free(ptxt_bits);
  free(ctxt_bits);
  free(key_bits);
  free(rcvrd_bits);
  return 0;
}
