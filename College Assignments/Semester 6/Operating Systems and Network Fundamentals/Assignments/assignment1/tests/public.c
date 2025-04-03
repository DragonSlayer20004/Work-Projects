#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <check.h>

#include "../fptr.h"
#include "../funs.h"

/* a single process from NEW to TRM */
START_TEST (C_run_add)
{
  int z = run_add (10, 2);
  ck_assert_int_eq (z, 12);
}
END_TEST

START_TEST (B_compile_parameter)
{
  int z = run_func (10, 2, sub);
  ck_assert_int_eq (z, 8);
}
END_TEST

void public_tests (Suite *s)
{
  TCase *tc_public = tcase_create ("Public");
  tcase_add_test (tc_public, C_run_add);
  tcase_add_test (tc_public, B_compile_parameter);
  suite_add_tcase (s, tc_public);
}

