#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "statemodel.h"
#include "stringmodel.h"

static void AdvancePointer (fsm_t *);
static void AllocateBuffer (fsm_t *);
static void AppendCharacter (fsm_t *);
static void ReplaceCharacter (fsm_t *);
static void SyntaxError (fsm_t *);
static state_t transition (fsm_t *, event_t, action_t *, action_t *);

// HINT: The type declarations in statemodel.h include
// unions. See https://www.learn-c.org/en/Unions for an
// introduction to unions. This example shows how
// to set up your transition table, much like Assignment 2:

static strst_t const transitions[NSTR_STATES][NSTR_EVENTS] = {
  // OPEN_QUOTE  CLOSE_QUOTE NON_CTRL  BACKSLASH ESC_CHAR  NO_ESC
  { BUILDING, NON_STR, NON_STR, NON_STR, NON_STR, NON_STR },   // STR_INIT
  { NON_STR, STR_FINISH, BUILDING, ESCAPE, NON_STR, NON_STR }, // BUILDING
  { NON_STR, NON_STR, NON_STR, NON_STR, BUILDING, STR_ERROR }, // ESCAPE
  { NON_STR, NON_STR, NON_STR, NON_STR, NON_STR, NON_STR },    // STR_FINISH
  { NON_STR, NON_STR, NON_STR, NON_STR, NON_STR, NON_STR }     // STR_ERROR
};

// Then create a table of effect functions.
static action_t const effectTransitions[NSTR_STATES][NSTR_EVENTS] = {
  // OPEN_QUOTE       CLOSE_QUOTE      NON_CTRL          BACKSLASH ESC_CHAR
  // NO_ESC
  { &AllocateBuffer, NULL, NULL, NULL, NULL, NULL },             // STR_INIT
  { NULL, &AdvancePointer, &AppendCharacter, NULL, NULL, NULL }, // BUILDING
  { NULL, NULL, NULL, NULL, &ReplaceCharacter, &SyntaxError },   // ESCAPE
  { NULL, NULL, NULL, NULL, NULL, NULL },                        // STR_FINISH
  { NULL, NULL, NULL, NULL, NULL, NULL }                         // STR_ERROR
};

// Finally, create an array of entry functions.
static action_t const entryTransitions[NSTR_STATES] = {
  // STR_INIT, BUILDING, ESCAPE, STR_FINISH, STR_ERROR
  NULL, NULL, NULL, NULL, NULL
};

bool
initialize_str_fsm (fsm_t *fsm)
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
  // you will need to add .string to the end of the field name
  // because of the union types. For instance, you can set the
  // maximum valid event for this FSM as:

  fsm->type = STR;
  fsm->state.string = STR_INIT;
  fsm->max_valid_event.string = NIL_CHAR; // Helpful example we need.
  fsm->max_valid_state.string = NON_STR;
  fsm->transition = transition;

  // Pointers to the input text, including the current byte being processed
  fsm->input = NULL;
  fsm->current = NULL;
  return true;
}

/* String version of the handle_evenint function */
void
string_event (fsm_t *fsm, strevt_t event)
{
  // Declare an event_t, put event in it, and call handle_event().
  event_t newEvent;
  newEvent.string = event;

  handle_event (fsm, newEvent);
}
/* String version of the transition function */
static state_t
transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{
  state_t next_state;

  // Strings that are helpful for debugging
  // static const char * event_names[]
  //     = { "OPEN_QUOTE", "CLOSE_QUOTE", "NON_CTRL",
  //         "BACKSLASH", "ESC_CHAR", "NO_ESC", "NIL_CHAR" };
  // static const char *state_names[]
  //     = { "STR_INIT", "BUILDING", "ESCAPE", "STR_FINISH",
  //         "STR_ERROR", "NON_STR" };

  /* If the state/event combination is bad, return current state */
  if (fsm->state.string > NON_STR || event.string > NIL_CHAR)
    {
      next_state.string = NON_STR;
      return next_state;
    }

  /* Look up the effect in the tables */
  *effect = effectTransitions[fsm->state.string][event.string];

  /* Look up the entries in the tables */
  *entry = entryTransitions[transitions[fsm->state.string][event.string]];

  /* Look up the transitions in the tables */
  next_state.string = transitions[fsm->state.string][event.string];
  return next_state;
}

// /////////////////// EFFECT FUNCTIONS ////////////////////

/* Used to move beyond the quote at the end of the string */
static void
AdvancePointer (fsm_t *fsm)
{
  // Increment current location
  fsm->current++;
}

/* Create a dynamically allocated buffer for storing the string as it is
   being built. Note that you will need to modify the fsm_t struct declaration
   to include whatever fields you may need for managing the buffer. */
static void
AllocateBuffer (fsm_t *fsm)
{
  // Plus 1 to handle null terminator.
  fsm->string_buffer
      = (char *)malloc (strlen (fsm->input + 1) * sizeof (char));

  // Set current location to 0.
  fsm->current_string_buffer_location = 0;

  // Set start to null character.
  fsm->string_buffer[0] = '\0';
}

/* Append a character from the current string to a buffer */
static void
AppendCharacter (fsm_t *fsm)
{
  // Copy the current character into the buffer.
  fsm->string_buffer[fsm->current_string_buffer_location] = *fsm->current;
  fsm->string_buffer[fsm->current_string_buffer_location + 1] = '\0';

  // Move to the next position in the buffer.
  fsm->current_string_buffer_location++;

  // Advance the FSM’s current pointer to the next input character.
  fsm->current++;
}

/* Replaces a control sequence (\\ or \") by putting just the
   latter character into the buffer */
static void
ReplaceCharacter (fsm_t *fsm)
{
  AppendCharacter (fsm);
}

/* Reports an invalid escape-code character */
static void
SyntaxError (fsm_t *fsm)
{
  if (fsm->string_buffer != NULL)
    {
      if (debug)
        {
          printf ("Freeing string_buffer at %p\n", (void *)fsm->string_buffer);
        }
      free (fsm->string_buffer);
      fsm->string_buffer = NULL;
    }
  printf ("SYNTAX ERROR: '%c%c' is not a valid escape code\n",
          *(fsm->current - 1), *fsm->current);
}