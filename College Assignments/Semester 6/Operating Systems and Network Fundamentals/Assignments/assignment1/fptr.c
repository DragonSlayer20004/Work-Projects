#include <stdbool.h>
#include <stdint.h>

#include "funs.h"

/*
 * Takes two numbers and adds them. Do NOT add the numbers directly.
 * Instead, create a function pointer variable that points to the
 * add() function defined in funs.h. Use this function pointer on
 * the parameters passed to run_add().
 */
int
run_add (int x, int y)
{
  // Fix this to declare fun and make it point to add
  // fun = add;
  int (*fun) (int, int);
  fun = &add;
  return fun (x, y);
}

// Fix this so that run_func takes three parameters: two ints and a
// single function pointer. The function pointed to by the pointer should
// take two ints and return an int. The body of the function should call
// the passed function with the two provided values, returning the result.
int
run_func (int x, int y, int (*function) (int, int))
{
  return function (x, y);
}

// Fix this to declare get_func() so that it returns a function pointer.
// Note that get_func() should still take a single bool, but the function
// pointer that it returns takes two int parameters and returns an int.
// HINT: Recall that the function pointer syntax goes AROUND the rest of
// the syntactical components.
int (*get_func (bool choice)) (int, int)
{
  if (choice)
    {
      return &add;
    }
  else
    {
      return &sub;
    }
}
