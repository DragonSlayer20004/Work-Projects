#ifndef __statemodel_h__
#define __statemodel_h__

#include <stdbool.h>
#include <stdint.h>

extern bool debug; // Global variable that is used for debugging transitions

// String processing events
typedef enum
{
  OPEN_QUOTE,  // triggered by " at the beginning of a string
  CLOSE_QUOTE, // read a " character at the end
  NON_CTRL,    // read a non-" or \ character
  BACKSLASH,   // read a \ character
  ESC_CHAR,    // allowable escape character (\" \\ \n \t \r)
  NO_ESC,      // non-allowable escape
  NIL_CHAR
} strevt_t;
#define NSTR_EVENTS NIL_CHAR

// String processing states
typedef enum
{
  STR_INIT,   // beginning to build a string
  BUILDING,   // appending characters
  ESCAPE,     // encountered a backslash
  STR_FINISH, // successfully built a string
  STR_ERROR,  // encountered an invalid escape code
  NON_STR
} strst_t;
#define NSTR_STATES NON_STR

// Integer processing events
typedef enum
{
  ZERO,      // 0 [leading 0 for octal format]
  HYPHEN,    // - [leading - to negate value]
  NZ_DIGIT,  // 1-9 [leading means non-octal]
  DIGIT,     // 0-9 [at least one leading digit before]
  TERM_INT,  // end of number (triggered by , } whitespace or \0
  NON_DIGIT, // invalid character
  NIL_INT
} intevt_t;
#define NINT_EVENTS NIL_INT

// Integer processing states
typedef enum
{
  INT_INIT,   // beginning to build an integer value
  MAGNITUDE,  // building the magnitude of the value
  SIGN,       // encountered a leading - to make negative
  OCTAL,      // converting to octal format
  INT_FINISH, // successfully built a string
  INT_ERROR,  // encountered an invalid digit
  NON_INT
} intst_t;
#define NINT_STATES NON_INT

// Value processing events
typedef enum
{
  START_VALUE, // starting to look for an integer or string value
  WHITESPACE,  // whitespace character to skip over
  START_STR,   // found a beginning ", so build a string
  END_STR,     // string was built successfully
  START_INT,   // found leading - or digit, so build an integer
  END_INT,     // integer was built successfully
  BAD_VALUE,   // an error occurred while building the integer/string
  NIL_VAL
} valevt_t;
#define NVAL_EVENTS NIL_VAL

// Value processing states
typedef enum
{
  VAL_INIT,   // beginning to build a value
  VAL_SKIP,   // skipping over leading whitespace
  BUILD_STR,  // building a string because " was encountered
  BUILD_INT,  // building an integer based on leading - or digit
  VAL_FINISH, // successfully built a value
  VAL_ERROR,  // encountered an error while building
  NON_VAL
} valst_t;
#define NVAL_STATES NON_VAL

// Object processing events
typedef enum
{
  OPEN_CB,    // encountered a leading { to start an object
  OBJ_WS,     // whitespace character to skip over
  START_ID,   // encountered a " to start an identifier
  END_ID,     // identifier string was accepted
  BAD_ID,     // syntax error in the identifier string
  COLON,      // encountered the : between identifier and value
  NON_COLON,  // something that wasn't : where it should be
  GOOD_VALUE, // string or integer value was accepted
  OBJ_BV,     // value was not successfully built
  BAD_TOKEN,  // after value, something other than } or ,
  COMMA,      // encountered , between multiple key-value pairs
  CLOSE_CB,   // encountered } to close the object
  NIL_OBJ
} objevt_t;
#define NOBJ_EVENTS NIL_OBJ

// Object processing states
typedef enum
{
  OBJ_INIT,    // beginning to build a JSON object
  OBJ_SKIP,    // skipping over leading whitespace
  BUILD_ID,    // building the key-value pair identifier string
  PEND_VALUE,  // waiting on the : to start finding the value
  BUILD_VALUE, // building the value of the pair
  SCANNING,    // looking for more pairs or end-of-object } character
  OBJ_FINISH,  // successfully built an object
  OBJ_ERROR,   // encountered one of many errors
  NON_OBJ
} objst_t;
#define NOBJ_STATES NON_OBJ

// Used to specify which of the four FSMs to use
typedef enum
{
  STR,
  INT,
  VAL,
  OBJ,
} fsm_type_t;

// Any state_t must be one of the state types above
typedef union State
{
  strst_t string;
  intst_t integer;
  valst_t value;
  objst_t object;
} state_t;

// Any event_t must be one of the events above
typedef union Event
{
  strevt_t string;
  intevt_t integer;
  valevt_t value;
  objevt_t object;
} event_t;

// Needed for circular typedef. This lets action_t use fsm_t in its parameter
// list, while the struct fsm can use action_t as a field.
typedef struct fsm fsm_t;

// All entry, exit, and effect instances use the action type
typedef void (*action_t) (fsm_t *);

// Generic FSM structure with type, state, maximums for events and states,
// and a pointer to the transition function.
struct fsm
{
  fsm_type_t type;
  state_t state;
  event_t max_valid_event;
  state_t max_valid_state;
  state_t (*transition) (struct fsm *, event_t, action_t *, action_t *);

  // Pointers to the input text, including the current byte being processed
  char const *input;
  char const *current;

  // TODO: Extend this with additional fields you need to hold information
  // during the execution of your FSM.

  // Additional String
  char *string_buffer;
  int current_string_buffer_location;

  // Additional Integer
  int64_t finalInt;
  bool negative;
  bool Multiplier; // True for 10 false for 8.

  // Additional Value
  bool valueSuccess;
  int emptySpaceRead;
  bool is_string;

  // Additional Object
  char *identifierString;
  char *fullKeyString;
  int fullKeyStringLength;
  int fullKeyStringCurrentLocation;
};

// Used to free up the dynamically allocated transition/effect arrays
void free_fsm (fsm_t *fsm);

// Generic entry point for handling events
void handle_event (fsm_t *fsm, event_t event);

#endif
