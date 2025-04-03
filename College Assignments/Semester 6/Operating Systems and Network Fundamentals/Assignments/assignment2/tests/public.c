#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <check.h>

#include "../statemodel.h"
#include "../effects.h"

/* a single process from NEW to TRM */
START_TEST (MIN_one_proc_linear_state_only)
{
  fsm_t fsm;
  assert (initialize_fsm (&fsm));

  handle_event (&fsm, ADMIT);
  ck_assert_int_eq (fsm.state, RDY);

  handle_event (&fsm, DISPATCH);
  ck_assert_int_eq (fsm.state, RUN);

  handle_event (&fsm, EXIT);
  ck_assert_int_eq (fsm.state, TRM);
}
END_TEST

/* Testing helper effect functions */
START_TEST (FULL_reset_runtime)
{
  fsm_t fsm;
  assert (initialize_fsm (&fsm));
  reset_runtime (&fsm);
  ck_assert_int_eq (fsm.runtime, 0);
}
END_TEST

/* Test that set_live works */
START_TEST (FULL_set_live)
{
  fsm_t fsm;
  assert (initialize_fsm (&fsm));
  ck_assert_int_eq (fsm.live, false);

  set_live (&fsm);
  ck_assert_int_eq (fsm.live, true);
  ck_assert_int_eq (fsm.runtime, 0);
}
END_TEST

/* Test that kill process sets liveness to false */
START_TEST (FULL_kill_process)
{
  fsm_t fsm;
  assert (initialize_fsm (&fsm));
  ck_assert_int_eq (fsm.live, false);

  set_live (&fsm);
  ck_assert_int_eq (fsm.live, true);

  kill_proc (&fsm);
  ck_assert_int_eq (fsm.live, false);
  ck_assert_int_eq (fsm.runtime, 0);
}
END_TEST

/* a single process from NEW to TRM */
START_TEST (FULL_one_proc_linear)
{
  fsm_t fsm;
  assert (initialize_fsm (&fsm));
  ck_assert_int_eq (fsm.state, NEW);
  ck_assert_int_eq (fsm.runtime, 0);
  ck_assert_int_eq (fsm.live, false);

  handle_event (&fsm, ADMIT);
  ck_assert_int_eq (fsm.state, RDY);
  ck_assert_int_eq (fsm.runtime, 0);
  ck_assert_int_eq (fsm.live, true);

  handle_event (&fsm, DISPATCH);
  ck_assert_int_eq (fsm.state, RUN);
  ck_assert_int_eq (fsm.runtime, 1);

  handle_event (&fsm, EXIT);
  ck_assert_int_eq (fsm.state, TRM);
  ck_assert_int_eq (fsm.live, true);
}
END_TEST

void public_tests (Suite *s)
{
  TCase *tc_public = tcase_create ("Public");
  tcase_add_test (tc_public, MIN_one_proc_linear_state_only);
  tcase_add_test (tc_public, FULL_reset_runtime);
  tcase_add_test (tc_public, FULL_set_live);
  tcase_add_test (tc_public, FULL_kill_process);
  tcase_add_test (tc_public, FULL_one_proc_linear);
  suite_add_tcase (s, tc_public);
}

