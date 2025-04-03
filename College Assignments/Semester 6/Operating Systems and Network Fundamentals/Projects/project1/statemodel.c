#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "statemodel.h"

bool debug = false;

// Declarations
bool check_valid_state (state_t nextState, fsm_t *fsm);

/* Frees up dynamically allocated data inside the FSM as needed. */
void
free_fsm (fsm_t *fsm)
{
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
}

/* Handles events by calling the transition function inside the FSM.
   Should be called by specific functions such as string_event(),
   integer_event(), value_event(), and object_event(). */
void
handle_event (fsm_t *fsm, event_t event)
{
  // Throw error if fsm doesn't exist
  assert (fsm != NULL);

  // Throw error if transition wasn't correctly set.
  if (!fsm->transition)
    {
      fprintf (stderr, "Error: FSM transition function is NULL!\n");
      return;
    }

  // Look up the FSM's table. If invalid throw an error.
  action_t effect = NULL;
  action_t entry = NULL;

  state_t nextState = fsm->transition (fsm, event, &effect, &entry);

  // Update the FSM state if in a valid state.
  bool validState = check_valid_state (nextState, fsm);

  // Perform effect (if it exists) and change the state.
  if (effect != NULL)
    {
      effect (fsm);
    }

  // Update FSM state only if valid
  fsm->state = nextState;
  if (validState)
    {

      // Execute the entry action if it exists
      if (entry != NULL)
        {
          entry (fsm);
        }
    }
  else
    {
      if (debug)
        {
          printf ("Invalid state transition prevented.\n");
        }
    }
}

/*Identifies the current fsm type and checks that the nextstate of that type
  isn't a NON state returning false if it is and true if it isn't.*/
bool
check_valid_state (state_t nextState, fsm_t *fsm)
{
  // Check for type string.
  if (fsm->type == STR)
    {
      if (debug)
        {
          printf ("Next state (STRING): %d\n", nextState.string);
        }
      if (nextState.string != NON_STR)
        {
          return true;
        }
    }

  // Check for type integer.
  else if (fsm->type == INT)
    {
      if (debug)
        {
          printf ("Next state (INTEGER): %d\n", nextState.integer);
        }
      if (nextState.integer != NON_INT)
        {
          return true;
        }
    }

  // Check for type value.
  else if (fsm->type == VAL)
    {
      if (debug)
        {
          printf ("Next state (VALUE): %d\n", nextState.value);
        }
      if (nextState.value != NON_VAL)
        {
          return true;
        }
    }

  // Check for type object.
  else if (fsm->type == OBJ)
    {
      if (debug)
        {
          printf ("Next state (OBJECT): %d\n", nextState.object);
        }
      if (nextState.object != NON_OBJ)
        {
          return true;
        }
    }

  // Debug message for invalid state
  if (debug)
    {
      printf ("Invalid transition detected: FSM type = %d\n", fsm->type);
    }

  // Return false if none of the types are valid.
  return false;
}
