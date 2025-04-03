#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "effects.h"
#include "statemodel.h"

// TODO: Complete this table mapping state/events to the target state DONE
static state_t const transitions[NUMBER_OF_STATES][NUMBER_OF_EVENTS] = {
  // ADMIT DISPATCH SCHEDULE BLOCK UNBLOCK EXIT KILL NIL
  { RDY, NST, NST, NST, NST, NST, TRM, NST }, // NEW
  { NST, RUN, NST, BLK, NST, NST, TRM, NST }, // READY
  { NST, RUN, RDY, BLK, NST, TRM, NST, NST }, // RUNNING
  { NST, NST, NST, NST, RDY, NST, TRM, NST }, // BLOCKED
  { NST, NST, NST, NST, NST, NST, NST, NST }  // TERM
};

// TODO: Create a table of action_t mapping states/events to effect functions
static action_t const effectTransitions[NUMBER_OF_STATES][NUMBER_OF_EVENTS] = {
  // ADMIT DISPATCH SCHEDULE BLOCK UNBLOCK EXIT KILL NIL
  { &set_live, NULL, NULL, NULL, NULL, NULL, &kill_proc, NULL }, // NEW
  { NULL, NULL, NULL, NULL, NULL, NULL, &kill_proc, NULL },      // READY
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },            // RUNNING
  { NULL, NULL, NULL, NULL, &say_unblocked, NULL, &kill_proc,
    NULL },                                          // BLOCKED
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL } // TERM
};

// TODO: Create an array of action_t to map each state to its entry function
static action_t const entryTransitions[NUMBER_OF_STATES] = {
  // NEW             READY    RUNNING        BLOCKED       TERM
  &reset_runtime, NULL, &incr_runtime, &say_blocked, &print_stats
};

/* Given FSM instance and event, perform the table lookups.
   Return NST if the state/event combination is bad.
   Otherwise, store the appropriate effect and entry functions
   into the pointer parameters. Return the next state. */
static state_t
transition (fsm_t *fsm, event_t event, action_t *effect, action_t *entry)
{
  /* If the state/event combination is bad, return current state */
  if (fsm->state > NST || event > NIL)
    return NST;

  /* Look up the effect in the tables */
  *effect = effectTransitions[fsm->state][event];

  /* Look up the entries in the tables */
  *entry = entryTransitions[transitions[fsm->state][event]];

  /* Look up the transitions in the tables */
  return transitions[fsm->state][event];
}

/* Initialize FSM to contain a pointer to the transition function.
   Also set the live, runtime, and state values to defaults.
   Return true if successful. */
bool
initialize_fsm (fsm_t *fsm)
{
  if (fsm == NULL)
    {
      return false;
    }
  fsm->runtime = 0;
  fsm->state = NEW;
  fsm->transition = transition;
  fsm->live = false;
  return true;
}

/* Handle an event given an FSM configuration. Confirm that the event is valid.
   Call the transition function pointer stored in the FSM with appropriate
   arguments. Print information about the transition. Call effect functions and
   entry functions if they aren't NULL. Update the state. */
void
handle_event (fsm_t *fsm, event_t event)
{
  static const char *event_names[]
      = { "ADMIT",   "DISPATCH", "SCHEDULE", "BLOCK",
          "UNBLOCK", "EXIT",     "KILL",     "NIL" };
  static const char *state_names[]
      = { "NEW", "RDY", "RUN", "BLK", "TRM", "NST" };

  // Throw error if fsm doesn't exist.
  assert (fsm != NULL);

  // Throw error if event is out of range.
  assert (event <= NUMBER_OF_EVENTS);

  // Look up the FSM's table. If invalid throw an error.
  action_t effect = NULL;
  action_t entry = NULL;

  // Failure results in -1
  state_t nextState = fsm->transition (fsm, event, &effect, &entry);
  assert (nextState != -1);

  // Grab oldState before it changes
  state_t oldState = fsm->state;

  // Update the FSM state
  if (nextState != NST)
    {
      fsm->state = nextState;
    }

  // TODO: Use the following format string to print the current state name,
  // state name, the event name, and the new state name for valid transitions.
  printf ("%s.%s -> %s\n", state_names[oldState], event_names[event],
          state_names[nextState]);

  // Perform effect (if it exists) and change the state.
  if (effect != NULL)
    {
      effect (fsm);
    }

  // Execute the entry action if it exists
  if (entry != NULL && nextState != NST)
    {
      entry (fsm);
    }
}
