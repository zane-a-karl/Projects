#include "../hdr/utils.h"

void
check_fopen (FILE* f) {

  if ( f == NULL ) {
    perror("Couldn't open file");
    exit(1);
  }
}

