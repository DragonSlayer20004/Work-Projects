#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intmodel.h"
#include "objmodel.h"
#include "parser.h"
#include "statemodel.h"
#include "stringmodel.h"
#include "valmodel.h"

static void ActivateString (fsm_t *);
static void ActivateValue (fsm_t *);
static void SetIdent (fsm_t *);
static void AdvancePointer (fsm_t *);
static void AppendKeyValuePair (fsm_t *);
static void SyntaxError (fsm_t *);
int count_digits (fsm_t *fsm, int64_t num);
static state_t transition (fsm_t *, event_t, action_t *, action_t *);

// Set up appropriate tables as in stringmodel.c.
static objevt_t const transitions[NOBJ_STATES][NOBJ_EVENTS] = {
  // OPEN_CB   OBJ_WS      START_ID  END_ID      BAD_ID     COLON NON_COLON
  // GOOD_VALUE  OBJ_BV     BAD_TOKEN  COMMA     CLOSE_CB
  { OBJ_SKIP, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ,
    NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ }, // OBJ_INIT
  { NON_OBJ, OBJ_SKIP, BUILD_ID, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ,
    NON_OBJ, OBJ_ERROR, NON_OBJ, NON_OBJ }, // OBJ_SKIP
  { NON_OBJ, NON_OBJ, NON_OBJ, PEND_VALUE, OBJ_ERROR, NON_OBJ, NON_OBJ,
    NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ }, // BUILD_ID
  { NON_OBJ, PEND_VALUE, NON_OBJ, NON_OBJ, NON_OBJ, BUILD_VALUE, OBJ_ERROR,
    NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ }, // PEND_VALUE
  { NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, SCANNING,
    OBJ_ERROR, NON_OBJ, NON_OBJ, NON_OBJ }, // BUILD_VALUE
  { NON_OBJ, SCANNING, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ,
    NON_OBJ, OBJ_ERROR, OBJ_SKIP, OBJ_FINISH }, // SCANNING
  { NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ,
    NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ }, // OBJ_FINISH
  { NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ,
    NON_OBJ, NON_OBJ, NON_OBJ, NON_OBJ } // OBJ_ERROR
};

// Then create a table of effect functions.
static action_t const effectTransitions[NOBJ_STATES][NOBJ_EVENTS] = {
  // OPEN_CB  OBJ_WS   START_ID END_ID     BAD_ID        COLON NON_COLON
  // GOOD_VALUE           OBJ_BV        BAD_TOKEN     COMMA    CLOSE_CB
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL }, // OBJ_INIT
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, SyntaxError, NULL,
    NULL }, // OBJ_SKIP
  { NULL, NULL, NULL, &SetIdent, &SyntaxError, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL }, // BUILD_ID
  { NULL, NULL, NULL, NULL, NULL, &AdvancePointer, &SyntaxError, NULL, NULL,
    NULL, NULL, NULL }, // PEND_VALUE
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, &AppendKeyValuePair,
    &SyntaxError, NULL, NULL, NULL }, // BUILD_VALUE
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &SyntaxError, NULL,
    NULL }, // SCANNING
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL }, // OBJ_FINISH
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL } // OBJ_ERROR
};

// Finally, create an array of entry functions.
static action_t const entryTransitions[NOBJ_STATES] = {
  // OBJ_INIT, OBJ_SKIP, BUILD_ID,             PEND_VALUE BUILD_VALUE,
  // SCANNING, OBJ_FINISH,  OBJ_ERROR
  NULL, NULL, &ActivateString, NULL, &ActivateValue, NULL, NULL, NULL
};

bool
initialize_obj_fsm (fsm_t *fsm)
{
  // See the hint in stringmodel.c.
  if (fsm == NULL)
    {
      return false;
    }

  // Set string null defaults
  fsm->string_buffer = NULL;
  fsm->identifierString = NULL;
  fsm->fullKeyString = NULL;

  // Note that, when setting other fields in the struct,
  // you will need to add .object to the end of the field name

  fsm->type = OBJ;
  fsm->state.object = OBJ_INIT;
  fsm->max_valid_event.object = NIL_OBJ;
  fsm->max_valid_state.object = NON_OBJ;
  fsm->transition = transition;

  // Pointers to the input text, including the current byte being processed
  fsm->input = NULL;
  fsm->current = NULL;

  // fullKeyString malloc
  fsm->fullKeyStringLength = 512;
  fsm->fullKeyString
      = (char *)malloc (fsm->fullKeyStringLength * sizeof (char));

  // Set current location to 0.
  fsm->fullKeyStringCurrentLocation = 0;

  // Set start to null character.
  fsm->fullKeyString[0] = '\0';

  // Additional Fields
  fsm->negative = false;

  return true;
}

static state_t
transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{
  state_t next_state;

  // Strings that are helpful for debugging
  // static const char * event_names[]
  //     = { "OPEN_CB", "OBJ_WS",    "START_ID",  "END_ID",
  //         "BAD_ID",  "COLON",     "NON_COLON", "GOOD_VALUE",
  //         "OBJ_BV",  "BAD_TOKEN", "COMMA",     "CLOSE_CB",
  //         "NIL_OBJ" };
  // static const char *state_names[]
  //     = { "OBJ_INIT",   "OBJ_SKIP",    "BUILD_ID",
  //         "PEND_VALUE", "BUILD_VALUE", "SCANNING",
  //         "OBJ_FINISH", "OBJ_ERROR",   "NON_OBJ" };
  // if (debug)
  //   printf ("%s.%s -> %s\n", state_names[current],
  //           event_names[event], state_names[next]);

  /* If the state/event combination is bad, return current state */
  if (fsm->state.object > NON_OBJ || event.object > NIL_OBJ)
    {
      next_state.object = NON_OBJ;
      return next_state;
    }

  /* Look up the effect in the tables */
  *effect = effectTransitions[fsm->state.object][event.object];

  /* Look up the entries in the tables */
  *entry = entryTransitions[transitions[fsm->state.object][event.object]];

  /* Look up the transitions in the tables */
  next_state.object = transitions[fsm->state.object][event.object];
  return next_state;
}

/* Object version of the handle_event function */
void
object_event (fsm_t *fsm, objevt_t event)
{
  // Declare an event_t, put event in it, and call handle_event().
  event_t newEvent;
  newEvent.object = event;

  handle_event (fsm, newEvent);
}

// /////////////////// EFFECT FUNCTIONS ////////////////////
// In AppendKeyValuePair, use a combination of strncat() and
// snprintf() to create format strings like the following:
//   printf ("KEYS[%s] = %" PRId64 "\n", ...
//   printf ("KEYS[%s] = %s\n", ...
// These strings should be concatenated with previous key-value
// pairs and stored somewhere that the accept_object can
// retrieve them later. Note that you can use realloc() to
// resize an existing dynamically
// allocated string to make space to concatenate.

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
                  success ? "true" : "fasle");
        }

      // Grab result if successful
      if (fsm->valueSuccess)
        {
          if (debug)
            {
              printf ("String result is: %s String length is %ld\n", result,
                      strlen (result));
            }
          fsm->string_buffer = result;
          result = NULL;

          if (stringfsm.string_buffer != NULL)
            {
              if (debug)
                {
                  printf ("Freeing stringfsm string_buffer at %p\n",
                          (void *)stringfsm.string_buffer);
                }
              free (stringfsm.string_buffer);
              stringfsm.string_buffer = NULL;
            }
          else
            {
              if (debug)
                {
                  printf ("stringfsm string_buffer is already NULL\n");
                }
            }
          // Add 2 for missing '"'
          fsm->current += (strlen (fsm->string_buffer) + 2);
        }
    }
}

static void
ActivateValue (fsm_t *fsm)
{
  fsm->valueSuccess = false;
  fsm_t valuefsm;
  char *result = "";

  // Create the fsm.
  if (initialize_val_fsm (&valuefsm))
    {
      // Initialize input and current
      valuefsm.input = fsm->input;
      valuefsm.current = fsm->current;

      bool isString;
      int64_t intResult;

      // start the fsm.
      bool success = accept_value (&valuefsm, &isString, &result, &intResult);
      fsm->valueSuccess = success;

      fsm->is_string = isString;

      // Grab result if successful
      if (isString)
        {
          if (fsm->valueSuccess)
            {
              if (debug)
                {
                  printf ("String result is: %s String length is %ld\n",
                          result, strlen (result));
                  printf ("Empty space increase %d\n",
                          valuefsm.emptySpaceRead);
                }

              fsm->string_buffer = result;
              result = NULL;

              // Add 2 for missing '"' and any extra whitespace.
              fsm->current += (strlen (fsm->string_buffer) + 2
                               + valuefsm.emptySpaceRead);

              if (valuefsm.string_buffer != NULL)
                {
                  if (debug)
                    {
                      printf ("Freeing valuefsm string_buffer at %p\n",
                              (void *)valuefsm.string_buffer);
                    }
                  free (valuefsm.string_buffer);
                  valuefsm.string_buffer = NULL;
                }
              else
                {
                  if (debug)
                    {
                      printf ("valuefsm string_buffer is already NULL\n");
                    }
                }
            }
        }
      else
        {
          if (fsm->valueSuccess)
            {
              int digits = count_digits (&valuefsm, intResult);
              if (debug)
                {
                  printf ("Value result is: %ld String length is %d\n",
                          intResult, digits);
                  printf ("Empty space increase %d\n",
                          valuefsm.emptySpaceRead);
                }

              fsm->finalInt = intResult;

              // If negative number increment.
              if (intResult < 0)
                {
                  fsm->negative = true;
                  fsm->current++;
                }

              // Add any extra whitespace.
              fsm->current += (digits + valuefsm.emptySpaceRead);
            }
        }
    }
}

static void
SetIdent (fsm_t *fsm)
{
  fsm->identifierString = fsm->string_buffer;
  fsm->string_buffer = NULL;
}

static void
AdvancePointer (fsm_t *fsm)
{
  // Increment current location past colon.
  fsm->current++;
}

/*
  AppendKeyValuePair: Appends the key-value pair in a specified
  format to a string containing all pairs, using realloc() to
  grow the result string
*/
static void
AppendKeyValuePair (fsm_t *fsm)
{
  if (debug)
    {
      printf ("Starting keyValuePair.\n");
    }
  int stringLength = 0;
  // Calculate string length. Plus 20 to count for extra text
  if (fsm->is_string)
    {
      stringLength
          = strlen (fsm->string_buffer) + strlen (fsm->identifierString) + 20;
    }
  else
    {
      stringLength = count_digits (fsm, fsm->finalInt)
                     + strlen (fsm->identifierString) + 20;
    }

  if (debug)
    {
      printf ("Key length: %d\tCurrent string length: %d\n", stringLength,
              fsm->fullKeyStringLength);
    }

  // If stringlength is too big expand the size of fullKeyString.
  if (stringLength + fsm->fullKeyStringCurrentLocation
      > fsm->fullKeyStringLength)
    {
      if (debug)
        {
          printf ("Expanded string length.\n");
        }
      fsm->fullKeyStringLength *= 2;
      fsm->fullKeyString = (char *)realloc (
          fsm->fullKeyString, sizeof (char) * fsm->fullKeyStringLength);
    }
  if (debug)
    {
      printf ("Starting to combine.\n");
    }
  // Combine everything together.
  strcat (fsm->fullKeyString, "KEYS[");
  strcat (fsm->fullKeyString, fsm->identifierString);
  strcat (fsm->fullKeyString, "] = ");
  if (fsm->is_string)
    {
      strcat (fsm->fullKeyString, fsm->string_buffer);
    }
  else
    {
      char *buffer = (char *)malloc (
          sizeof (char) * 64); // Temporary buffer to hold formatted string
      snprintf (buffer, sizeof (char) * 64, "%ld", fsm->finalInt);
      strcat (fsm->fullKeyString, buffer);

      if (buffer != NULL)
        {
          if (debug)
            {
              printf ("Freeing buffer at %p\n", (void *)buffer);
            }
          free (buffer);
          buffer = NULL;
        }
      else
        {
          if (debug)
            {
              printf ("buffer is already NULL\n");
            }
        }
      if (debug)
        {
          printf ("Combined string with integer.\n");
        }
    }
  strcat (fsm->fullKeyString, "\n");

  if (fsm->string_buffer != NULL)
    {
      if (debug)
        {
          printf ("Freeing string_buffer at %p\n", (void *)fsm->string_buffer);
        }
      free (fsm->string_buffer);
      fsm->string_buffer = NULL;
    }
  else
    {
      if (debug)
        {
          printf ("string_buffer is already NULL\n");
        }
    }

  if (fsm->identifierString != NULL)
    {
      if (debug)
        {
          printf ("Freeing identifierString at %p\n",
                  (void *)fsm->identifierString);
        }
      free (fsm->identifierString);
      fsm->identifierString = NULL;
    }
  else
    {
      if (debug)
        {
          printf ("identifierString is already NULL\n");
        }
    }

  // Set current location
  fsm->fullKeyStringCurrentLocation = strlen (fsm->fullKeyString);
}

static void
SyntaxError (fsm_t *fsm)
{
  // For syntax errors, if there is a newline character ('\n'),
  // replace it with a null byte ('\0'), then use this format
  // string:

  // Find the first occurrence of '\n' in fsm->current
  char *newline_pos = strchr (fsm->current, '\n');

  // If '\n' is found, replace it with '\0'
  if (newline_pos)
    {
      *newline_pos = '\0';
    }

  if (fsm->fullKeyString != NULL)
    {
      if (debug)
        {
          printf ("Freeing fullKeyString at %p\n", (void *)fsm->fullKeyString);
        }
      free (fsm->fullKeyString);
      fsm->fullKeyString = NULL;
    }
  else
    {
      if (debug)
        {
          printf ("fullKeyString is already NULL\n");
        }
    }

  if (fsm->string_buffer != NULL)
    {
      if (debug)
        {
          printf ("Freeing string_buffer at %p\n", (void *)fsm->string_buffer);
        }
      free (fsm->string_buffer);
      fsm->string_buffer = NULL;
    }
  else
    {
      if (debug)
        {
          printf ("string_buffer is already NULL\n");
        }
    }

  if (fsm->identifierString != NULL)
    {
      if (debug)
        {
          printf ("Freeing identifierString at %p\n",
                  (void *)fsm->identifierString);
        }
      free (fsm->identifierString);
      fsm->identifierString = NULL;
    }
  else
    {
      if (debug)
        {
          printf ("identifierString is already NULL\n");
        }
    }

  printf ("SYNTAX ERROR: Could not process text beginning at '%s'\n",
          fsm->current);
}

int
count_digits (fsm_t *fsm, int64_t num)
{
  if (num < 0)
    { // Special case for negative
      if (debug)
        {
          printf ("Number is negative\n");
        }
      num *= -1;
    }
  else
    {
      if (debug)
        {
          printf ("Number is not negative\n");
        }
    }
  if (num == 0)
    return 1; // Special case for 0
  int count = 0;
  while (num != 0)
    {
      num /= 10;
      count++;
    }
  if (debug)
    {
      printf ("Digits count is: %d\n", count);
    }
  return count;
}