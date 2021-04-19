#ifndef _UTILS_ALL_MODES_H_
#define _UTILS_ALL_MODES_H_

#include <assert.h>
#include <math.h> // floor
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
  ECB, CBC, CFB, OFB, CTR
} CIPHERMODE;

typedef struct CipherInfo {
  bool *key;
  char *i_file;
  char *o_file;
  int b_len;
  int i_len;
  int k_len;
  int n_blocks;
  CIPHERMODE type;
} CipherInfo;

void
ascii_to_bits (bool **bit_str, // O
	       char *ascii_str);

void
bits_to_ascii (char **ascii_str, // O
	       bool *bit_str,
	       int bit_str_len);

void
print_bits (bool *arr,
	    int len);

int
set_cipher_mode (char *mode);

void
get_num_input_blocks (int *n_blocks, // O
		      char *input_filename,
		      int block_length);

void
get_key_length (int *key_len, // O
		char *key_filename);

void
get_key (char **key, // O
	 char *key_filename);

void
handle_return_FILE_error (char *error_str,
			  FILE* f);

void
handle_return_int_error (char *error_str,
			 int ret_val);

#endif // _UTILS_ALL_MODES_H_
