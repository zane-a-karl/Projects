#define DEBUG 1

#include "../../electronic-code-book/ecb.h"
#include "../../utils.h"
#include <stdlib.h>

bool
test_ecb (char **filename) {

  //  TSTART;
  
  char *key = "z@nezan3";
  int key_len = strlen(key);
  printf("##### KEY #########\n");
  printf("%s\n", key);

  FILE* fin = fopen(filename, 'r');
  int ch;
  int ptxt_len;
  printf("##### PTXT #########\n");
  do {
    ch = fgetc(fin);
    if ( feof(fin) == ch ) {
      break;
    }
    if ( ferror(fin) == ch ) {
      perror("Error reading from file\n");
      exit(1);
    }
    ++ptxt_len;
  } while (1);
  fseek(fin, 0, SEEK_SET);

  int block_width = 8;

  bool *ptxt_bits = (bool *)calloc(ptxt_len*8, sizeof *ptxt_bits);
  ascii_to_bits(&ptxt_bits, ptxt);
  bool *key_bits = (bool *)calloc(key_len*8, sizeof *key_bits);
  ascii_to_bits(&key_bits, key);

  /* printf("##### PTXT_BITS #########\n"); */
  /* print_bits(ptxt_bits, ptxt_len); */

  /* printf("##### KEY_BITS #########\n"); */
  /* print_bits(key_bits, key_len); */


  bool *ctxt_bits = (bool *)calloc(ptxt_len*8, sizeof *ctxt_bits);
  ecb(&ctxt_bits,
      ptxt_bits,
      key_bits,
      block_width,
      ptxt_len*8,
      key_len*8);
  /* printf("##### CTXT_BITS #########\n"); */
  /* print_bits(ctxt_bits, ptxt_len); */

  printf("##### CTXT #########\n");
  char *ctxt = (char *)calloc(ptxt_len, sizeof *ctxt);
  bits_to_ascii(&ctxt, ctxt_bits, ptxt_len*8);
  for (int i = 0; i < ptxt_len; ++i) {
    printf("%d", ctxt[i]);
    if (i==ptxt_len-1) {
      printf("\n");
    } else {
      printf("  ");
    }
  }
  printf("##### RECOVERED PTXT #########\n");
  bool *rcvrd_bits = (bool *)calloc(ptxt_len*8, sizeof *rcvrd_bits);
  threaded_ecb(&rcvrd_bits,
      ctxt_bits,
      key_bits,
      block_width,
      ptxt_len*8,
      key_len*8);
  char *rcvrd = (char *)calloc(ptxt_len, sizeof *rcvrd);
  bits_to_ascii(&rcvrd, rcvrd_bits, ptxt_len*8);
  printf("%s\n", rcvrd);

  //  TSTAMP;

  if (strcmp(rcvrd, ptxt) == 0) {
    return true;
  }

  fclose(fin);

  free(ctxt);
  free(rcvrd);
  free(ptxt_bits);
  free(ctxt_bits);
  free(key_bits);
  free(rcvrd_bits);

  return false;
}

int
main (int argc,
      char **argv) {

  if ( test_ecb(argv[1]) ) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}
