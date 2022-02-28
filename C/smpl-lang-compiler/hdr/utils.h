#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>

void
handle_return_FILE_error (char *error_str,
													FILE* f);

void
handle_return_int_error (char *error_str,
												 int ret_val);

#endif //_UTILS_H_
