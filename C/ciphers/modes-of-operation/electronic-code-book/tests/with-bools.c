#include "../../utils.h"
#include "../ecb.h"
#include <stdio.h>
#include <stdlib.h>


bool
test_ecb_bools() {

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
  ecb_bools(&ctxt_bits,
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
  ecb_bools(&rcvrd_bits,
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
  return true;
}

int
main() {

  if ( test_ecb_bools() ) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}
