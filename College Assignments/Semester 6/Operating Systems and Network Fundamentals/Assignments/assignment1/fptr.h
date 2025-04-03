#ifndef __COMP3400_FPTR__
#define __COMP3400_FPTR__

#include <stdbool.h>

int run_add (int, int);
int run_func (int, int, int (*function) (int, int));
void *get_func (bool);

#endif
