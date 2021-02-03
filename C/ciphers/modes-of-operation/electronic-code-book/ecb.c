#include "ecb.h"


/**
 * TODO for next time:
 *  - take advantage of the parallelization opportuity that ecb creates using
 the pthread library.
 *  - do a shared memory key and then socket io message test with ecb.
 *  - write note comments for ecb to remind you what ecb is (maybe include
 diagrams).
*/

/** ECB OVERVIEW
    Assume the BLOCK_LENGTH = 2

    ENCRYPTION:
            ----------------------------------------------
  PTXT ---> |    P0    |    P1    |     P2    |    P3    |
            ----------------------------------------------
                 |          |           |          |
                 |          |           |          |
               -----      -----       -----      -----
               |XOR|      |XOR|       |XOR|      |XOR|
               -----      -----       -----      -----
                 |          |           |          |
                 |          |           |          |
                 V          V           V          V
            ---------------------------------------------
  KEY  ---> |    K0    |    K1    |    K0    |    K1    |
            ---------------------------------------------
                 |          |           |          |
                 |          |           |          |
                 V          V           V          V
            ----------------------------------------------
  CTXT ---> |    C0    |    C1    |     C2    |    C3    |
            ----------------------------------------------
                 |          |           |          |
  DECRYPTION:    |          |           |          |
               -----      -----       -----      -----
               |XOR|      |XOR|       |XOR|      |XOR|
               -----      -----       -----      -----
                 |          |           |          |
                 |          |           |          |
                 V          V           V          V
            ---------------------------------------------
  KEY  ---> |    K0    |    K1    |    K0    |    K1    |
            ---------------------------------------------
                 |          |           |          |
                 |          |           |          |
                 V          V           V          V
            ----------------------------------------------
  PTXT ---> |    P0    |    P1    |     P2    |    P3    |
            ----------------------------------------------
 */

/** Encode and Decode are the same function, just with the
 * inputs moved around.
 * Notice that should the key length > block_len only the first
 * block_len bits of the key will be used. Should key length <
 * block_len then the final (block_len-key_len) bits will be
 * filled by the first (block_len-key_len) bits of the key.
 */
void
ecb_bools (bool **output, // O
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

/** Encode and Decode are the same function, just with the
 * inputs swapped.
 */
void
ecb_chars (char **output, // O
	   char *input,
	   char *key,
	   int block_len,
	   int input_len,
	   int key_len) {

  char *modified_key = (char *)calloc(block_len,
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
