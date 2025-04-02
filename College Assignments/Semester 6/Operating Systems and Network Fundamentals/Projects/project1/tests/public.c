#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <check.h>

#include "../statemodel.h"
#include "../stringmodel.h"
#include "../intmodel.h"
#include "../valmodel.h"
#include "../objmodel.h"
#include "../parser.h"

// Test that accept_string accepts a basic "hello" string
START_TEST (PART_string)
{
  char *input = "\"hello\"";
  fsm_t string;
  ck_assert (initialize_str_fsm (&string));
  string.input = input;
  string.current = input;
  char *str = NULL;
  ck_assert (accept_string (&string, &str));
  ck_assert (str != NULL);
  ck_assert_str_eq (str, "hello");
  free_fsm (&string);
  free (str);
}
END_TEST

// Test that accept_integer accepts normal decimal
START_TEST (PART_integer)
{
  char *input = "1234";
  fsm_t integer;
  ck_assert (initialize_int_fsm (&integer));
  integer.input = input;
  integer.current = input;
  int64_t value = 0;
  ck_assert (accept_integer (&integer, &value));
  ck_assert_int_eq (value, 1234);
  free_fsm (&integer);
}
END_TEST

// Test that accept_value accepts a basic "hello" string
START_TEST (MIN_value_string)
{
  char *input = "\"hello\"";
  fsm_t value;
  ck_assert (initialize_val_fsm (&value));
  value.input = input;
  value.current = input;
  bool is_string = false;
  char *str = NULL;
  int64_t integer = 0;
  ck_assert (accept_value (&value, &is_string, &str, &integer));
  ck_assert (str != NULL);
  ck_assert_str_eq (str, "hello");
  ck_assert_int_eq (integer, 0);
  free_fsm (&value);
  free (str);
}
END_TEST

// Test that accept_object accepts a basic {"hello":"goodbye"} string pair
START_TEST (FULL_object_string)
{
  char *input = "{\"hello\":\"goodbye\"}";
  fsm_t object;
  ck_assert (initialize_obj_fsm (&object));
  object.input = input;
  object.current = input;
  char *str = NULL;
  debug = true;
  bool result = accept_object (&object, &str);
  debug = false;
  ck_assert (result);
  ck_assert (str != NULL);
  ck_assert_str_eq (str, "KEYS[hello] = goodbye\n");
  free_fsm (&object);
  free (str);
}
END_TEST

void public_tests (Suite *s)
{
  TCase *tc_public = tcase_create ("Public");
  tcase_add_test (tc_public, PART_string);
  tcase_add_test (tc_public, PART_integer);
  tcase_add_test (tc_public, MIN_value_string);
  tcase_add_test (tc_public, FULL_object_string);
  suite_add_tcase (s, tc_public);
}

