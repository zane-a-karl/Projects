#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifdef DEBUG
  #define PRINTD(M, ...) fprintf(stderr, "\n[INFO] (%s:%d) "M"\n\n",\
				 __FILE__, __LINE__, ##__VA_ARGS__)
#else
  #define PRINTD(M, ...)
#endif

#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <time.h>

static struct timespec t1,t2;
static double sec;

#define TSTART clock_gettime(CLOCK_MONOTONIC, &t1);
#define TSTAMP clock_gettime(CLOCK_MONOTONIC, &t2); \
              sec = (t2.tv_sec - t1.tv_sec) + \
		(t2.tv_nsec - t1.tv_nsec) / \
		1000000000.0; \
	      fprintf(stderr,"Line:%5d, Time = %f\n",__LINE__,sec);

typedef enum {
  ECB, CBC, CFB, OFB, CTR;
} CIPHERMODE;

typedef struct Cipher {
  FILE *output;
  FILE *input;
  bool *key;
  int input_len;
  int block_len;
  int key_len;
  CIPHERMODE type;
} Cipher;

void
ascii_to_bits (bool **bit_str, // O
	       char *ascii_str);

void
bits_to_ascii (char **ascii_str, // O
	       bool *bit_str,
	       int bit_str_len);

static inline void
print_bits (bool *arr,
	    int len) {

  for (int i = 0; i < len; ++i) {
    for (int j = 0; j < 8; ++j) {
      printf("%d", arr[8*i + j]);
    }
    printf("\n");
  }
}

#endif // _UTILS_H_
