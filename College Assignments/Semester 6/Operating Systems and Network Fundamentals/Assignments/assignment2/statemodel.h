#ifndef __statemodel_h__
#define __statemodel_h__

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

// Events
typedef enum
{
  ADMIT,    // admit a process into the system
  DISPATCH, // dispatch a process to run
  SCHEDULE, // move running process back to the queue
  BLOCK,    // block a process until an unblock occurs
  UNBLOCK,  // unblock a process back into the queue
  EXIT,     // running process is choosing to exit
  KILL,     // system kills non-running process
  NIL       // non-event (cause no transition)
} event_t;
#define NUMBER_OF_EVENTS (NIL + 1)

// Possible states
typedef enum
{
  NEW, // a new process
  RDY, // ready to run
  RUN, // the (only) running process
  BLK, // blocked (waiting)
  TRM, // terminated (cannot emerge)
  NST  // invalid non-state
} state_t;
#define NUMBER_OF_STATES NST

// Needed for circular typedef. This lets action_t use fsm_t in its parameter
// list, while the struct fsm can use action_t as a field.
typedef struct fsm fsm_t;

// All entry, exit, and effect instances use the action type
typedef void (*action_t) (fsm_t *);

// Each FSM instance contains a current state
struct fsm
{
  state_t state;    // the process state
  uint32_t runtime; // the number of times process is in RUN
  bool live;        // true if admitted and not killed
  // pointer to transition function
  state_t (*transition) (struct fsm *, event_t, action_t *, action_t *);
};

// Initialize the FSM to set up its tables
bool initialize_fsm (fsm_t *fsm);

// Execute a state transition for the passed machine
void handle_event (fsm_t *fsm, event_t event);

#endif
