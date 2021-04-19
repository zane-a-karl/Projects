#ifndef _ENCRYPT_FILE_INPUT_H_
#define _ENCRYPT_FILE_INPUT_H_

#include "../utils-all-modes.h"

#include <math.h> // ceil
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

typedef struct ThreadData {
  bool *input_block;
  bool *output_block;
  char *ptxt_block;
  char *ctxt_block;
  int idx;
  CipherInfo CI;
  FILE *fout;
} ThreadData;

static int n_enc_threads;
// Following needed for write_mutex synchrony
static int curr_block_idx;
static bool *thread_is_pending;
pthread_mutex_t write_mutex;
pthread_cond_t sequential_cond;

bool
encrypt_using_ecb (char *input_file,
		   char *key_file,
		   int b_width);

void
encrypt (CipherInfo CI);

void
encrypt_block (bool **o_block,
	       bool *i_block,
	       CipherInfo CI);

void
threaded_encrypt (CipherInfo CI);

void *
threaded_encrypt_block (void *thread_args);

void *
watch_next_block_idx (void *thread_args);

#endif // _ENCRYPT_FILE_INPUT_H_
