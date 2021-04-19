#include "utils-all-modes.h"


/** Converts the ascii str into a bit str of length =
 * (strlen * bitwidth) stored in big endian.
 * Assume bit_str has already been calloc'd.
 */
void
ascii_to_bits (bool **bit_str, // O
	       char *ascii_str) {

  int bits_per_char = 8;
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
	       int bit_str_len) {

  int bits_per_char = 8;
  for (int i = 0; i < bit_str_len; ++i) {
    (*ascii_str)[i/bits_per_char] +=
      bit_str[i] * (1<<(bits_per_char-(i%bits_per_char)-1));
  }
}

void
print_bits (bool *arr,
	    int len) {

  int rows = floor(len/8);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < 8; ++j) {
      printf("%d", arr[8*i + j]);
    }
    printf("\n");
  }
}

/** Converts a string into its CIPHERMODE enum equivalent
 */
int
set_cipher_mode (char *mode) {
  if (strcmp(mode, "ECB") == 0) {
    return ECB;
#define othermode(m) } else if (strcmp(mode, #m) == 0) { return m;
    othermode(CBC);
    othermode(CFB);
    othermode(OFB);
    othermode(CTR);
    #undef othermode
  } else {
    printf("Invalid mode\n");
    assert(0);
  }
}

void
get_num_input_blocks (int *n_blocks, // O
		      char *input_filename,
		      int block_length) {

  FILE *fin = fopen(input_filename, "r");
  handle_return_FILE_error("Error fopen input_file", fin);
  int ch, ret_val, input_length = 0;
  do {

    ch = fgetc(fin);
    if ( feof(fin) != 0 ) {
      break;
    }
    ret_val = ferror(fin);
    handle_return_int_error("Error ferror input_file", ret_val);
    ++input_length;

  } while (1);
  ret_val = fclose(fin);
  handle_return_int_error("Error fclose key_file", ret_val);

  (*n_blocks) = ceil((double)input_length / (block_length/8));
}

void
get_key_length (int *key_length, // O
		char *key_filename) {

  FILE *fin = fopen(key_filename, "r");
  handle_return_FILE_error("Error fopen key_file", fin);
  int ch, ret_val;
  do {

    ch = fgetc(fin);
    if ( feof(fin) != 0 ) {
      break;
    }
    ret_val = ferror(fin);
    handle_return_int_error("Error ferror key_file", ret_val);
    ++(*key_length);

  } while (1);
  ret_val = fclose(fin);
  handle_return_int_error("Error fclose key_file", ret_val);
}

void
get_key (char **key, // O
	 char *key_filename) {

  FILE *fin = fopen(key_filename, "r");
  handle_return_FILE_error("Error fopen key_file", fin);
  int i = 0, ch, ret_val;
  do {

    ch = fgetc(fin);
    if ( feof(fin) != 0 ) {
      break;
    }
    ret_val = ferror(fin);
    handle_return_int_error("Error ferror key_file", ret_val);
    (*key)[i++] = ch;

  } while (1);
  ret_val = fclose(fin);
  handle_return_int_error("Error fclose key_file", ret_val);
}

void
handle_return_FILE_error (char *error_str,
			  FILE* f) {

  if ( f == NULL ) {
    printf("%s\n", error_str);
    perror(NULL);
    exit(1);
  }
}

void
handle_return_int_error (char *error_str,
			 int ret_val) {

  if ( ret_val != 0 ) {
    printf("%s\n", error_str);
    perror(NULL);
    exit(1);
  }
}
