#include "encrypt-file-input.h"


bool
encrypt_using_ecb (char *input_filename,
		   char *key_filename,
		   int b_len) {

  bool status = false;
  int bits__char = 8;

  char *ascii_key;
  bool *key;
  int ascii_key_length;
  int key_length;
  int num_input_blocks;

  printf("########### KEY ###########\n");
  get_key_length(&ascii_key_length, key_filename);
  ascii_key = (char *)calloc(ascii_key_length, sizeof *ascii_key);
  get_key(&ascii_key, key_filename);
  key_length = ascii_key_length * bits__char;
  key = (bool *)calloc(key_length, sizeof *key);
  for (int i = 0; i < key_length; ++i) {
    ascii_key[i] -= 48;
  }
  ascii_to_bits(&key, ascii_key);
  print_bits(key, key_length);
  printf("###########################\n");

  printf("####### BLOCK WIDTH #######\n");
  printf("%14d\n", b_len);
  printf("###########################\n");

  get_num_input_blocks(&num_input_blocks, input_filename, b_len);

  CipherInfo CI;
  CI.key = key;
  CI.i_file = input_filename;
  CI.o_file = "output-files/ECB.txt";
  CI.b_len = b_len;
  CI.i_len = 0;
  CI.k_len = key_length;
  CI.n_blocks = num_input_blocks;

  /* encrypt(CI); */
  threaded_encrypt(CI);
  status = true;

  free(ascii_key);
  free(key);

  return status;
}

void
encrypt (CipherInfo CI) {

  FILE *fin = fopen(CI.i_file, "r");
  handle_return_FILE_error("Error fopen i_file", fin);
  FILE *fout = fopen(CI.o_file, "w");
  handle_return_FILE_error("Error fopen o_file", fout);
  bool *input_block = (bool *)calloc(CI.b_len, sizeof (bool));
  bool *output_block = (bool *)calloc(CI.b_len, sizeof (bool));
  char *ptxt_block = (char *)calloc(CI.b_len/8, sizeof *ptxt_block);
  char *ctxt_block = (char *)calloc(CI.b_len/8, sizeof *ctxt_block);
  int n_read, n_write, ret_val;
  do {
    n_read = fread(ptxt_block, sizeof (char), CI.b_len/8, fin);
    if ( n_read == 0 && feof(fin) != 0 ) {
      break;
    }
    ret_val = ferror(fin);
    handle_return_int_error("Error ferror i_file", ret_val);

    CI.i_len += n_read;
    ascii_to_bits(&input_block, ptxt_block);
    encrypt_block(&output_block, input_block, CI);
    bits_to_ascii(&ctxt_block, output_block, CI.b_len);

    n_write = fwrite(ctxt_block,
		     sizeof (char),
		     strlen(ctxt_block),
		     fout);
    if ( feof(fout) != 0 ) {
      break;
    }
    ret_val = ferror(fout);
    handle_return_int_error("Error ferror o_file", ret_val);

    memset(input_block,  0, CI.b_len * sizeof *input_block);
    memset(output_block, 0, CI.b_len * sizeof *output_block);
    memset(ptxt_block,   0, CI.b_len * sizeof *ptxt_block);
    memset(ctxt_block,   0, CI.b_len * sizeof *ctxt_block);
  } while (1);

  ret_val = fclose(fin);
  handle_return_int_error("Error fclose i_file", ret_val);
  ret_val = fclose(fout);
  handle_return_int_error("Error fclose o_file", ret_val);

  free(input_block);
  free(output_block);
  free(ptxt_block);
  free(ctxt_block);
}

void
encrypt_block (bool **o_block,
	       bool *i_block,
	       CipherInfo CI) {

  for (int i = 0; i < CI.b_len; ++i) {
    (*o_block)[i] = i_block[i];
    if (CI.k_len >= CI.b_len) {
      (*o_block)[i] ^= CI.key[i % CI.b_len];
    } else {
      (*o_block)[i] ^= CI.key[i % CI.k_len];
    }
  }
}

void
threaded_encrypt (CipherInfo CI) {

  n_enc_threads = 4;
  int ret_val = 0;
  pthread_t threads[n_enc_threads];
  pthread_t watch_thread;
  pthread_attr_t attr;
  ThreadData TD[n_enc_threads];

  FILE *fout = fopen(CI.o_file, "w");
  handle_return_FILE_error("Error fopen o_file", fout);

  ret_val = pthread_mutex_init(&write_mutex, NULL);
  handle_return_int_error("Error mutex_init write_mutex", ret_val);
  ret_val = pthread_cond_init(&sequential_cond, NULL);
  handle_return_int_error("Error cond_init seq_cond", ret_val);

  curr_block_idx = 0;
  thread_is_pending = (bool *)calloc(n_enc_threads, sizeof (bool));
  thread_is_pending[0] = true;

  /* For portability, explicitly create threads in joinable state */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for (int i = 0; i < n_enc_threads; ++i) {

    TD[i].input_block = (bool *)calloc(CI.b_len, sizeof (bool));
    TD[i].output_block = (bool *)calloc(CI.b_len, sizeof (bool));
    TD[i].ptxt_block = (char *)calloc(CI.b_len/8, sizeof (char));
    TD[i].ctxt_block = (char *)calloc(CI.b_len/8, sizeof (char));
    TD[i].idx = i;
    TD[i].CI = CI;
    TD[i].fout = fout;
    ret_val = pthread_create(&threads[i],
			     &attr,
			     threaded_encrypt_block,
			     (void *)&TD[i]);
    handle_return_int_error("Error pthread_create", ret_val);
  }
  ret_val = pthread_create(&watch_thread,
			   &attr,
			   watch_next_block_idx,
			   (void *)&CI);
  handle_return_int_error("Error pthread_create", ret_val);

  for (int i = 0; i < n_enc_threads; ++i) {
    ret_val = pthread_join(threads[i], NULL);
    handle_return_int_error("Error pthread_join", ret_val);
  }

  ret_val = fclose(fout);
  handle_return_int_error("Error fclose o_file", ret_val);
  
  for (int i = 0; i < n_enc_threads; ++i) {
    free(TD[i].input_block);
    free(TD[i].output_block);
    free(TD[i].ptxt_block);
    free(TD[i].ctxt_block);
  }

  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&write_mutex);
  pthread_cond_destroy(&sequential_cond);
}

void *
threaded_encrypt_block (void *thread_args) {

  ThreadData *TD = (ThreadData *)thread_args;

  FILE *fin = fopen(TD->CI.i_file, "r");
  handle_return_FILE_error("Error fopen i_file", fin);

  int n_read, n_write, ret_val;

  long offset =  TD->idx * TD->CI.b_len/8;
  ret_val = fseek(fin, offset, SEEK_SET);
  handle_return_int_error("Error fseek i_file SET", ret_val);

  do {

    n_read = fread(TD->ptxt_block,
		   sizeof (char),
		   TD->CI.b_len/8,
		   fin);

    if ( n_read == 0 && feof(fin) != 0 ) {
      break;
    }
    ret_val = ferror(fin);
    handle_return_int_error("Error ferror i_file", ret_val);

    ascii_to_bits(&TD->input_block, TD->ptxt_block);
    encrypt_block(&TD->output_block, TD->input_block, TD->CI);
    bits_to_ascii(&TD->ctxt_block, TD->output_block, TD->CI.b_len);
    thread_is_pending[TD->idx] = true;
    
    ret_val = pthread_mutex_lock(&write_mutex);
    handle_return_int_error("Error mutex_lock write_mtx", ret_val);

    while ( TD->idx != (curr_block_idx % n_enc_threads) ) {
      ret_val = pthread_cond_wait(&sequential_cond, &write_mutex);
      handle_return_int_error("Error cond_wait seq_cond", ret_val);
    }

    /* fwrite() will write exactly the number of bytes you give it.
       No more, no less. So if you haven't filled a later portion
       of the array fwrite() will just add gibberish to it.*/
    n_write = fwrite(TD->ctxt_block,
		     sizeof (char),
		     strlen(TD->ctxt_block), /* Very Important */
		     TD->fout);
    /* printf("Writing block %d...\n", curr_block_idx); */

    thread_is_pending[TD->idx] = false;
    ++curr_block_idx;
  
    ret_val = pthread_mutex_unlock(&write_mutex);
    handle_return_int_error("Error mutex_unlock write_mtx", ret_val);

    if ( feof(TD->fout) != 0 ) {
      break;
    }
    ret_val = ferror(TD->fout);
    handle_return_int_error("Error ferror o_file", ret_val);

    memset(TD->input_block,  0, TD->CI.b_len * sizeof (bool));
    memset(TD->output_block, 0, TD->CI.b_len * sizeof (bool));
    memset(TD->ptxt_block,   0, TD->CI.b_len * sizeof (char));
    memset(TD->ctxt_block,   0, TD->CI.b_len * sizeof (char));
    offset = (n_enc_threads-1) * (TD->CI.b_len/8);
    ret_val = fseek(fin, offset, SEEK_CUR);
    handle_return_int_error("Error fseek i_file CUR", ret_val);

  } while (1);

  /* printf("FINISHED Thread #%d!!!\n", TD->idx); */

  ret_val = fclose(fin);
  handle_return_int_error("Error fclose i_file", ret_val);

  return NULL;
}

void *
watch_next_block_idx (void *thread_args) {

  int ret_val = 0;
  CipherInfo *CI = (CipherInfo *)thread_args;

  while (1) {
    if (curr_block_idx == CI->n_blocks) {
      return NULL;
    }
    ret_val = pthread_mutex_lock(&write_mutex);
    handle_return_int_error("Error mutex_lock watch_mtx", ret_val);

    if ( thread_is_pending[curr_block_idx % n_enc_threads] ) {
      pthread_cond_broadcast(&sequential_cond);
    }
    ret_val = pthread_mutex_unlock(&write_mutex);
    handle_return_int_error("Error mutex_unlock watch_mtx", ret_val);
  }
}
