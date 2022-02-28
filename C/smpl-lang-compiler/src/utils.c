#include "../hdr/utils.h"


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
