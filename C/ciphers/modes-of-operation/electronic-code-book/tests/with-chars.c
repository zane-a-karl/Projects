#include "../../utils.h"
#include "../ecb.h"
#include <stdio.h>
#include <stdlib.h>


bool
test_ecb_chars() {

  char *key = "z@nezan3";
  int key_len = strlen(key);
  char *ptxt = "Attack at dawn!";
  int ptxt_len = strlen(ptxt);
  int block_width = 8;

  printf("##### PTXT_INTS #########\n");
  printf("%s\n", ptxt);
  for (int i = 0; i < ptxt_len; ++i) {
    printf("%d", ptxt[i]);
    if (i==ptxt_len-1) {
      printf("\n");
    } else {
      printf("  ");
    }
  }

  printf("##### KEY_INTS #########\n");
  printf("%s\n", key);
  for (int i = 0; i < key_len; ++i) {
    printf("%d", key[i]);
    if (i==key_len-1) {
      printf("\n");
    } else {
      printf("  ");
    }
  }

  printf("##### CTXT_INTS #########\n");
  char *ctxt = (char *)calloc(ptxt_len, sizeof *ctxt);
  ecb_chars(&ctxt, ptxt, key, block_width, ptxt_len, key_len);
  for (int i = 0; i < ptxt_len; ++i) {
    printf("%d", ctxt[i]);
    if (i==ptxt_len-1) {
      printf("\n");
    } else {
      printf("  ");
    }
  }

  printf("##### RECOVERED PTXT #########\n");
  char *rcvrd = (char *)calloc(ptxt_len, sizeof *rcvrd);
  ecb_chars(&rcvrd, ctxt, key, block_width, ptxt_len, key_len);
  printf("%s\n", rcvrd);
  for (int i = 0; i < ptxt_len; ++i) {
    printf("%d", rcvrd[i]);
    if (i==ptxt_len-1) {
      printf("\n");
    } else {
      printf("  ");
    }
  }

  free(ctxt);
  free(rcvrd);

  return true;
}

int
main() {

  if ( test_ecb_chars() ) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}
