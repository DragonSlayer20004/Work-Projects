#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "fptr.h"
#include "funs.h"

int
main (int argc, char **argv)
{
  // Call a function that internally sets and uses a function pointer
  int z = run_add (5, 3);
  printf ("5 + 3 = %d\n", z);

  // Call a function that passes a function pointer
  z = run_func (5, 3, sub);
  printf ("5 - 3 = %d\n", z);

  // TODO: Change this so that f is declared properly so that the commented
  // lines can be compiled and run for the integration tests.
  int (*f) (int, int) = get_func (true);
  printf ("1 + 2 = %d\n", f (1, 2));
  f = get_func (false);
  printf ("1 - 2 = %d\n", f (1, 2));

  return 0;
}
