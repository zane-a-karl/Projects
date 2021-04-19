#include "electronic-code-book/encrypt-file-input.h"
#include "utils-all-modes.h"

#include <assert.h>
#include <stdio.h>

/** To Test:
 *  [1] gcc TEST-ANY-MODE.c utils-all-modes.c electronic-code-book/encrypt-file-input.c -Wall -g
 *  [2] time ./a.out ECB input-files/<file> key-files/<file> <blk_len>
 *  [3] check that output-files/ECB.txt matches input-files/<file> for the time being the key is just 0s
 */


/** This file is going to be pretty cool. It's whole purpose is to
 * allow for a general test to be performed of any one of the cipher
 * modes of operation. E.g., Say I want the content of some file
 * encrypted via ECB mode then all I have to do is pass the string
 * "ECB" and the <filename> of the file containing the content I
 * would like to encrypt and this file will choose the correct cipher
 * mode and output the encryption of the content in the file
 * (preferably to another file).
 *  The name of this file is currently "test.c" because it is meant
 * to make sure that the different modes are encrypting correctly.
 * However, I have a feeling that I'll need to make a "test-enc.c"
 * as well as a "test-dec.c" and use the output of the former as
 * input to the latter and then check against the original input
 * to "test-enc.c."
 */

bool
test_enc (char *cipher_mode,
	  char *input_filename,
	  char *key_filename,
	  char *block_length) {

  CIPHERMODE mode = set_cipher_mode(cipher_mode);
  int b_len;
  if ( 0 == sscanf(block_length, "%d", &b_len) ) {
    printf("Error parsing block width\n");
    exit(1);
  }
  assert(b_len >= 8 && b_len % 8 == 0);
  bool status = false;
  switch (mode) {
    case ECB:
      status = encrypt_using_ecb(input_filename, key_filename, b_len);
      break;
    case CBC:
      //      status = encrypt_using_cbc(input_filename, key_filename, b_len);
      break;
    case CFB:
      //      status = encrypt_using_cfb(input_filename, key_filename, b_len);
      break;
    case OFB:
      //      status = encrypt_using_ofb(input_filename, key_filename, b_len);
      break;
    case CTR:
      //      status = encrypt_using_ctr(input_filename, key_filename, b_len);
      break;
    default:
      printf("PLACE AN EASTER EGG HERE\n");
      break;
    }
  
  return status;
}

int
main (int argc,
      char **argv) {

  assert(argc >= 5);
  char *cipher_mode = argv[1];
  char *input_filename = argv[2];
  char *key_filename = argv[3];
  char *block_length = argv[4];
  if ( test_enc(cipher_mode, input_filename, key_filename, block_length) ) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }
  
  return 0;
}

