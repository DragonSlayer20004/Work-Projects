#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "intmodel.h"
#include "parser.h"
#include "statemodel.h"
#include "stringmodel.h"
#include "valmodel.h"

static void ActivateString (fsm_t *);
static void ActivateInteger (fsm_t *);
static void SyntaxError (fsm_t *);
static state_t transition (fsm_t *, event_t, action_t *, action_t *);

// Set up appropriate tables as in stringmodel.c.
static valevt_t const transitions[NVAL_STATES][NVAL_EVENTS] = {
  // START_VALUE  WHITESPACE  START_STR  END_STR     START_INT  END_INT
  // BAD_VALUE
  { VAL_SKIP, NIL_VAL, NIL_VAL, NIL_VAL, NIL_VAL, NIL_VAL,
    NIL_VAL }, // VAL_INIT
  { NIL_VAL, VAL_SKIP, BUILD_STR, NIL_VAL, BUILD_INT, NIL_VAL,
    VAL_ERROR }, // VAL_SKIP
  { NIL_VAL, NIL_VAL, NIL_VAL, VAL_FINISH, NIL_VAL, NIL_VAL,
    VAL_ERROR }, // BUILD_STR
  { NIL_VAL, NIL_VAL, NIL_VAL, NIL_VAL, NIL_VAL, VAL_FINISH,
    VAL_ERROR }, // BUILD_INT
  { NIL_VAL, NIL_VAL, NIL_VAL, NIL_VAL, NIL_VAL, NIL_VAL,
    NIL_VAL }, // VAL_FINISH
  { NIL_VAL, NIL_VAL, NIL_VAL, NIL_VAL, NIL_VAL, NIL_VAL,
    NIL_VAL } // VAL_ERROR
};

// Then create a table of effect functions.
static action_t const effectTransitions[NVAL_STATES][NVAL_EVENTS] = {
  // START_VALUE     WHITESPACE      START_STR        END_STR   START_INT
  // END_INT     BAD_VALUE
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL },         // VAL_INIT
  { NULL, NULL, NULL, NULL, NULL, NULL, &SyntaxError }, // VAL_SKIP
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL },         // BUILD_STR
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL },         // BUILD_INT
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL },         // VAL_FINISH
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL }          // VAL_ERROR
};

// Finally, create an array of entry functions.
static action_t const entryTransitions[NVAL_STATES] = {
  // VAL_INIT, VAL_SKIP, BUILD_STR,            BUILD_INT, VAL_FINISH, VAL_ERROR
  NULL, NULL, &ActivateString, &ActivateInteger, NULL, NULL
};

bool
initialize_val_fsm (fsm_t *fsm)
{
  if (fsm == NULL)
    {
      return false;
    }

  // Set string null defaults
  fsm->string_buffer = NULL;
  fsm->identifierString = NULL;
  fsm->fullKeyString = NULL;

  // Note that, when setting other fields in the struct,
  // you will need to add .value to the end of the field name

  fsm->type = VAL;
  fsm->state.value = VAL_INIT;
  fsm->max_valid_event.value = NIL_VAL;
  fsm->max_valid_state.value = NON_VAL;
  fsm->transition = transition;

  // Pointers to the input text, including the current byte being processed
  fsm->input = NULL;
  fsm->current = NULL;

  // Additional fields.
  fsm->emptySpaceRead = 0;
  fsm->is_string = false;
  return true;
}

static state_t
transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{
  state_t next_state;

  // Strings that are helpful for debugging
  // static const char * event_names[]
  //     = { "START_VALUE", "WHITESPACE", "START_STR",
  //         "END_STR",     "START_INT",  "END_INT",
  //         "BAD_VALUE",   "NIL_VAL" };
  // static const char *state_names[]
  //     = { "VAL_INIT",  "VAL_SKIP",   "BUILD_STR",
  //         "BUILD_INT", "VAL_FINISH", "VAL_ERROR",
  //         "NON_VAL" };
  // if (debug)
  //   printf ("%s.%s -> %s\n", state_names[current],
  //           event_names[event], state_names[next]);

  /* If the state/event combination is bad, return current state */
  if (fsm->state.value > NON_VAL || event.value > NIL_VAL)
    {
      next_state.value = NON_VAL;
      return next_state;
    }

  /* Look up the effect in the tables */
  *effect = effectTransitions[fsm->state.value][event.value];

  /* Look up the entries in the tables */
  *entry = entryTransitions[transitions[fsm->state.value][event.value]];

  /* Look up the transitions in the tables */
  next_state.value = transitions[fsm->state.value][event.value];
  return next_state;
}

/* Value version of the handle_event function */
void
value_event (fsm_t *fsm, valevt_t event)
{
  // Declare an event_t, put event in it, and call handle_event().
  event_t newEvent;
  newEvent.value = event;

  handle_event (fsm, newEvent);
}

// /////////////////// EFFECT FUNCTIONS ////////////////////

static void
ActivateString (fsm_t *fsm)
{
  fsm->valueSuccess = false;
  fsm_t stringfsm;
  char *result = "";

  // Create the fsm.
  if (initialize_str_fsm (&stringfsm))
    {
      // Initialize input and current
      stringfsm.input = fsm->input;
      stringfsm.current = fsm->current;

      // start the fsm.
      bool success = accept_string (&stringfsm, &result);
      fsm->valueSuccess = success;

      if (debug)
        {
          printf ("If passed string successful check 2: %s\n",
                  success ? "true" : "false");
        }

      // Grab result if successful
      if (fsm->valueSuccess)
        {
          if (debug)
            {
              printf ("String result is: %s\n", result);
            }
          fsm->string_buffer = result;
        }
    }
}

static void
ActivateInteger (fsm_t *fsm)
{
  fsm->valueSuccess = false;
  fsm_t integerfsm;
  int64_t intResult;

  // Create the fsm.
  if (initialize_int_fsm (&integerfsm))
    {
      // Initialize input and current
      integerfsm.input = fsm->input;
      integerfsm.current = fsm->current;

      // start the fsm.
      bool success = accept_integer (&integerfsm, &intResult);
      fsm->valueSuccess = success;

      if (debug)
        {
          printf ("If passed integer successful check 2: %s\n",
                  success ? "true" : "false");
        }

      // Grab result if successful
      if (fsm->valueSuccess)
        {
          if (debug)
            {
              printf ("Integer result is: %ld\n", intResult);
            }
          fsm->finalInt = intResult;
        }
    }
}

static void
SyntaxError (fsm_t *fsm)
{
  // Use this format string for syntax errors:
  printf ("SYNTAX ERROR: '%c' is an invalid token\n", *fsm->current);
}