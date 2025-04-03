#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "intmodel.h"
#include "statemodel.h"

static void SetNegative (fsm_t *);
static void SetMultiplier_10 (fsm_t *);
static void SetMultiplier_8 (fsm_t *);
static void MultAndAdd (fsm_t *);
static void SyntaxError (fsm_t *);
static state_t transition (fsm_t *fsm, event_t event, action_t *effect,
                           action_t *entry);

// Set up appropriate tables as in stringmodel.c.
static intevt_t const transitions[NINT_STATES][NINT_EVENTS] = {
  // ZERO     HYPHEN   NZ_DIGIT   DIGIT      TERM_INT    NON_DIGIT
  { OCTAL, SIGN, MAGNITUDE, NON_INT, NON_INT, NON_INT },           // INT_INIT
  { NON_INT, NON_INT, NON_INT, MAGNITUDE, INT_FINISH, INT_ERROR }, // MAGNITUDE
  { OCTAL, NON_INT, MAGNITUDE, NON_INT, NON_INT, INT_ERROR },      // SIGN
  { OCTAL, NON_INT, MAGNITUDE, NON_INT, INT_FINISH, INT_ERROR },   // OCTAL
  { NON_INT, NON_INT, NON_INT, NON_INT, NON_INT, NON_INT }, // INT_FINISH
  { NON_INT, NON_INT, NON_INT, NON_INT, NON_INT, NON_INT }  // INT_ERROR
};

// Then create a table of effect functions.
static action_t const effectTransitions[NINT_STATES][NINT_EVENTS] = {
  // ZERO  HYPHEN        NZ_DIGIT           DIGIT        TERM_INT NON_DIGIT
  { NULL, &SetNegative, &SetMultiplier_10, NULL, NULL, NULL }, // INT_INIT
  { NULL, NULL, NULL, &MultAndAdd, NULL, &SyntaxError },       // MAGNITUDE
  { NULL, NULL, &SetMultiplier_10, NULL, NULL, &SyntaxError }, // SIGN
  { NULL, NULL, &SetMultiplier_8, NULL, NULL, &SyntaxError },  // OCTAL
  { NULL, NULL, NULL, NULL, NULL, NULL },                      // INT_FINISH
  { NULL, NULL, NULL, NULL, NULL, NULL }                       // INT_ERROR
};

// Finally, create an array of entry functions.
static action_t const entryTransitions[NINT_STATES] = {
  // INT_INIT, MAGNITUDE, SIGN, OCTAL, INT_FINISH, INT_ERROR
  NULL, NULL, NULL, NULL, NULL, NULL
};

bool
initialize_int_fsm (fsm_t *fsm)
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
  // you will need to add .integer to the end of the field name
  fsm->type = INT;
  fsm->state.integer = INT_INIT;
  fsm->max_valid_event.integer = NIL_INT;
  fsm->max_valid_state.integer = NON_INT;
  fsm->transition = transition;

  // Pointers to the input text, including the current byte being processed
  fsm->input = NULL;
  fsm->current = NULL;
  return true;

  // Additional fields.
  fsm->finalInt = 0;
  fsm->negative = false;
}

static state_t
transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{
  state_t next_state;

  // Strings that are helpful for debugging
  // static const char *event_names[]
  //     = { "ZERO",     "HYPHEN",    "NZ_DIGIT", "DIGIT",
  //         "TERM_INT", "NON_DIGIT", "NIL_INT" };
  // static const char *state_names[]
  //     = { "INT_INIT",   "MAGNITUDE", "SIGN",   "OCTAL",
  //         "INT_FINISH", "INT_ERROR", "NON_INT" };
  /* If the state/event combination is bad, return current state */
  if (fsm->state.integer > NON_INT || event.integer > NIL_INT)
    {
      next_state.integer = NON_INT;
      return next_state;
    }

  /* Look up the effect in the tables */
  *effect = effectTransitions[fsm->state.integer][event.integer];

  /* Look up the entries in the tables */
  *entry = entryTransitions[transitions[fsm->state.integer][event.integer]];

  /* Look up the transitions in the tables */
  next_state.integer = transitions[fsm->state.integer][event.integer];
  return next_state;
}

/* Integer version of the handle_event function */
void
integer_event (fsm_t *fsm, intevt_t event)
{
  // Declare an event_t, put event in it, and call handle_event().
  event_t newEvent;
  newEvent.integer = event;

  handle_event (fsm, newEvent);
}

// /////////////////// EFFECT FUNCTIONS ////////////////////

static void
SetNegative (fsm_t *fsm)
{
  fsm->negative = true;
}

static void
SetMultiplier_10 (fsm_t *fsm)
{
  fsm->Multiplier = true;
}

static void
SetMultiplier_8 (fsm_t *fsm)
{
  fsm->Multiplier = false;
}

static void
MultAndAdd (fsm_t *fsm)
{
  // Do multiplier
  if (fsm->Multiplier)
  {
    fsm->finalInt = fsm->finalInt * 10;
  }
  else
  {
    fsm->finalInt = fsm->finalInt * 8;
  }

  fsm->finalInt += (*fsm->current - '0');
  fsm->current++;
}

static void
SyntaxError (fsm_t *fsm)
{
  // Use this format string for syntax errors:
  printf ("SYNTAX ERROR: '%c' is not a valid digit\n", *fsm->current);
}