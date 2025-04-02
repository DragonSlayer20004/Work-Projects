#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "intmodel.h"
#include "objmodel.h"
#include "statemodel.h"
#include "stringmodel.h"
#include "valmodel.h"

/* Begins at fsm->current and tries to identify a string in the given
   input data. A string begins and ends with ". In between, only two
   special escape sequences are allowed: \\ and \". If the string is
   successfully built, the result pointer will be changed to point to
   a dynamically allocated copy and return true. Return false if any
   error occurs. The escape sequences in the input will be replaced with
   the appropriate character (e.g., \\ in the input becomes \ in the
   result string).

   (HINT: Note that the effect functions do not have
   access to local variables here; instead, you should extend the fsm_t
   declaration to include helper variables, such as a buffer to copy the
   input bytes into.) */
bool
accept_string (fsm_t *fsm, char **result)
{
  if (!fsm || !fsm->current)
    {
      if (debug)
        {
          printf ("fsm or fsm->current doesn't exist\n");
        }
      return false;
    }

  while (fsm->state.string < STR_FINISH)
    {
      if (!fsm->current || *fsm->current == '\0')
        {
          if (debug)
            {
              printf ("Reached end of input unexpectedly\n");
            }
          return false;
        }
      switch (fsm->state.string)
        {
        case STR_INIT:
          if (debug)
            {
              printf ("Current state: %d, First char: %c\n", fsm->state.string,
                      fsm->current[0]);
            }
          if (fsm->current[0] == '"')
            {
              string_event (fsm, OPEN_QUOTE);
              fsm->current++;
            }
          else
            {
              string_event (fsm, NIL_CHAR);
            }

          if (debug)
            {
              printf ("Current state: %d, Current char: %c\n",
                      fsm->state.string, *fsm->current);
            }
          break;

        case BUILDING:
          if (*fsm->current == '\\')
            {
              string_event (fsm, BACKSLASH);
              fsm->current++;
            }
          else if (*fsm->current == '"')
            {
              string_event (fsm, CLOSE_QUOTE);
            }
          else
            {
              string_event (fsm, NON_CTRL);
            }
          if (debug)
            {
              printf ("Current state: %d, Current char: %c\n",
                      fsm->state.string, *fsm->current);
            }
          break;

        case ESCAPE:
          if (*fsm->current == '\\' || *fsm->current == '"')
            {
              string_event (fsm, ESC_CHAR);
            }
          else
            {
              string_event (fsm, NO_ESC);
            }
          if (debug)
            {
              printf ("Current state: %d, Current char: %c\n",
                      fsm->state.string, *fsm->current);
            }
          break;

        default:
          break;
        }
    }

  // Run if successfully finished.
  if (fsm->state.string == STR_FINISH)
    {
      // Null terminate
      fsm->string_buffer[fsm->current_string_buffer_location] = '\0';
      *result = fsm->string_buffer;
      fsm->string_buffer = NULL;

      if (debug)
        {
          printf ("If passed string successful check 1: true\n");
        }
      return true;
    }
  return false;
}

/* Begins at fsm->current and tries to build a valid integer value. This
   function should accept both positive and negative numbers, and should
   allow leading 0s to indicate that the number is in octal format (e.g.,
   023 should print as the decimal value 19). Whitespace, the } character,
   the , character, and the null byte '\0' can be used to indicate the end
   of a number. Any other non-digit value should be considered a NON_DIGIT
   and result in a syntax error. If the number was successfully built, it
   should be copied into the location pointed to by the call-by-reference
   parameter value and the function should return true. Otherwise, return
   false. */
bool
accept_integer (fsm_t *fsm, int64_t *value)
{
  if (!fsm || !fsm->current)
    {
      if (debug)
        {
          printf ("fsm or fsm->current doesn't exist\n");
        }
      return false;
    }

  while (fsm->state.integer < INT_FINISH)
    {
      if (!fsm->current)
        {
          if (debug)
            {
              printf ("Reached end of input unexpectedly\n");
            }
          return false;
        }

      switch (fsm->state.integer)
        {
        case INT_INIT:
          fsm->negative = false;
          fsm->finalInt = 0;
          if (debug)
            {
              printf ("First state (INTEGER): %d, First char: %c\n",
                      fsm->state.integer, fsm->current[0]);
            }
          if (fsm->current[0] == '-')
            {
              integer_event (fsm, HYPHEN);
              fsm->current++;
            }
          else if (fsm->current[0] == '0')
            {
              integer_event (fsm, ZERO);
            }
          else
            {
              integer_event (fsm, NZ_DIGIT);
            }

          if (debug)
            {
              printf ("Current state (INTEGER): %d, Current char: %c\n",
                      fsm->state.integer, *fsm->current);
            }
          break;

        case MAGNITUDE:
          if (*fsm->current >= '0' && *fsm->current <= '9')
            {
              integer_event (fsm, DIGIT);
            }
          else if (*fsm->current == ' ' || *fsm->current == '}'
                   || *fsm->current == ',' || *fsm->current == '\0'
                   || *fsm->current == '\n')
            {
              if (debug)
                {
                  printf ("Integer is negative?: %s\n",
                          fsm->negative ? "true" : "false");
                }
              // Change to negative if needed.
              if (fsm->negative)
                {
                  fsm->finalInt *= -1;
                }
              integer_event (fsm, TERM_INT);
            }
          else
            {
              integer_event (fsm, NON_DIGIT);
            }
          if (debug)
            {
              printf ("Current state (INTEGER): %d, Current char: %c\n",
                      fsm->state.integer, *fsm->current);
            }
          break;

        case SIGN:
          if (*fsm->current >= '0' && *fsm->current <= '9')
            {
              integer_event (fsm, NZ_DIGIT);
            }
          else if (*fsm->current == '0')
            {
              integer_event (fsm, ZERO);
            }
          else
            {
              integer_event (fsm, NON_DIGIT);
            }
          if (debug)
            {
              printf ("Current state (INTEGER): %d, Current char: %c\n",
                      fsm->state.integer, *fsm->current);
            }
          break;

        case OCTAL:
          if (*fsm->current >= '0' && *fsm->current <= '9')
            {
              integer_event (fsm, NZ_DIGIT);
            }
          else if (*fsm->current == '0')
            {
              integer_event (fsm, ZERO);
              fsm->current++;
            }
          else if (*fsm->current == ' ' || *fsm->current == '}'
                   || *fsm->current == ',' || *fsm->current == '\0'
                   || *fsm->current == '\n')
            {
              if (debug)
                {
                  printf ("Integer is negative?: %s\n",
                          fsm->negative ? "true" : "false");
                }
              // Change to negative if needed.
              if (fsm->negative)
                {
                  fsm->finalInt *= -1;
                }
              integer_event (fsm, TERM_INT);
            }
          else
            {
              integer_event (fsm, NON_DIGIT);
            }
          if (debug)
            {
              printf ("Current state (INTEGER): %d, Current char: %c\n",
                      fsm->state.integer, *fsm->current);
            }
          break;

        default:
          break;
        }
    }

  if (fsm->state.integer == INT_FINISH)
    {
      *value = fsm->finalInt;
      return true;
    }
  return false;
}

/* Begins at fsm->current and tries to build a value that can be either
   a string or an integer. Skip over leading whitespace until either a
   leading ", -, or digit is encountered. The " indicates the value will
   be a string, whereas the others indicate an integer. Other characters
   should cause a BAD_VALUE event, triggering a syntax error message.
   Returns true only if a valid string or integer value was accepted.
   If a value was successully parsed, sets the is_string, string, and
   value call-by-reference parameters as appropriate. */
bool
accept_value (fsm_t *fsm, bool *is_string, char **string, int64_t *value)
{
  if (!fsm || !fsm->current)
    {
      if (debug)
        {
          printf ("fsm or fsm->current doesn't exist\n");
        }
      return false;
    }

  while (fsm->state.value < VAL_FINISH)
    {
      if (!fsm->current || *fsm->current == '\0')
        {
          if (debug)
            {
              printf ("Reached end of input unexpectedly\n");
            }
          return false;
        }

      switch (fsm->state.value)
        {
        case VAL_INIT:
          if (debug)
            {
              printf ("Current state: %d, First char: %c\n", fsm->state.value,
                      fsm->current[0]);
            }
          value_event (fsm, START_VALUE);
          fsm->emptySpaceRead = 0;

          if (debug)
            {
              printf ("Current state: %d, Current char: %c\n",
                      fsm->state.value, *fsm->current);
            }
          break;

        case VAL_SKIP:
          if (*fsm->current == ' ')
            {
              fsm->current++;
              fsm->emptySpaceRead++;
              value_event (fsm, WHITESPACE);
            }
          else if (*fsm->current == '"')
            {
              value_event (fsm, START_STR); // Check if string
            }
          else if (*fsm->current == '-'
                   || (*fsm->current >= '0' && *fsm->current <= '9'))
            {
              value_event (fsm, START_INT); // Check if integer
            }
          else
            {
              value_event (fsm, BAD_VALUE); // Failed to be either integer,
                                            // string or whitspace.
            }

          if (debug)
            {
              printf ("Current state: %d, Current char: %c\n",
                      fsm->state.value, *fsm->current);
            }
          break;

        case BUILD_STR:
          *is_string = true;
          fsm->is_string = true;
          value_event (fsm, END_STR);
          break;

        case BUILD_INT:
          *is_string = false;
          fsm->is_string = false;
          value_event (fsm, END_INT);
          break;

        default:
          break;
        }
    }

  if (debug)
    {
      printf ("Final state (VALUE) is %d when it needs to be %d\n",
              fsm->state.value, VAL_FINISH);
    }
  // Run if successfully finished.
  if (fsm->state.value == VAL_FINISH && fsm->valueSuccess)
    {
      if (debug)
        {
          printf ("Value is a string?: %s\n", *is_string ? "true" : "false");
        }

      if (*is_string)
        {
          *string = fsm->string_buffer;
        }
      else
        {
          // ADD INT BUFFER HERE
          *value = fsm->finalInt;
        }

      fsm->string_buffer = NULL;
      return true;
    }
  return false;
}

/* Begins at fsm->current and tries to build a valid JSON object. All JSON
   objects consist of key-value pairs, where the key is a string and the
   value can be either a string or integer. If more than one key-value pair
   is in the object, they are separated by commas. The following examples
   are all valid (note that whitespace is ignored):

     {"a":1}
     { "first": "second", "alpha": "beta"}
     { "integer": 1, "string": "one" }

   These are examples of bad objects (and the corresponding error events):

     { not : "good" }  -- BAD_TOKEN
     { "no \t allowed" : } -- BAD_ID
     { "alpha" } -- NON_COLON
     { "float" : 1.5 } -- BAD_VALUE
     { "a" : "b" : "c" } -- BAD_TOKEN
     { "a" : "b", } -- BAD_TOKEN

   All key-value pairs for an object will be concatenated as a string
   and returned via the keys call-by-reference parameter. The format
   should look exactly as (ignore the leading space):

     KEYS[integer] = 1
     KEYS[string] = one

   AppendKeyValuePair should append a new line (ending in '\n') for
   each key-value pair found. In the case above, the exact string
   returned would be:

     "KEYS[integer] = 1\nKEYS[string] = one\n"

   Return true if the object is successfully parsed, false otherwise.
   */
bool
accept_object (fsm_t *fsm, char **keys)
{
  if (!fsm || !fsm->current)
    {
      if (debug)
        {
          printf ("fsm or fsm->current doesn't exist\n");
        }
      return false;
    }

  while (fsm->state.object < OBJ_FINISH)
    {
      if (!fsm->current || *fsm->current == '\0')
        {
          if (debug)
            {
              printf ("Reached end of input unexpectedly\n");
            }
          return false;
        }

      switch (fsm->state.object)
        {
        case OBJ_INIT:
          if (debug)
            {
              printf ("First state (OBJECT): %d, First char: %c\n",
                      fsm->state.object, fsm->current[0]);
            }
          if (fsm->current[0] == '{')
            {
              fsm->current++;
              object_event (fsm, OPEN_CB);
            }
          else
            {
              return false; // Invalid start of object
            }
          if (debug)
            {
              printf ("Current state (OBJECT): %d, Current char: %c\n",
                      fsm->state.object, *fsm->current);
            }
          break;

        case OBJ_SKIP:
          if (*fsm->current == ' ' || *fsm->current == '\n')
            {
              fsm->current++;
              object_event (fsm, WHITESPACE);
            }
          else if (*fsm->current == '"')
            {
              object_event (fsm, START_ID);
            }
          else
            {
              if (debug)
                {
                  printf ("Bad Token event. Current char: %c\n",
                          *fsm->current);
                }
              object_event (fsm, BAD_TOKEN);
            }
          if (debug)
            {
              printf ("Current state (OBJECT): %d, Current char: %c\n",
                      fsm->state.object, *fsm->current);
            }
          break;

        case BUILD_ID:
          if (fsm->valueSuccess)
            {
              object_event (fsm, END_ID);
            }
          else
            {
              object_event (fsm, BAD_ID);
            }
          if (debug)
            {
              printf ("Current state (OBJECT): %d, Current char: %c\n",
                      fsm->state.object, *fsm->current);
            }
          break;

        case PEND_VALUE:
          if (*fsm->current == ' ')
            {
              fsm->current++;
              object_event (fsm, WHITESPACE);
            }
          else if (*fsm->current == ':')
            {
              object_event (fsm, COLON);
            }
          else
            {
              object_event (fsm, NON_COLON);
            }
          if (debug)
            {
              printf ("Current state (OBJECT): %d, Current char: %c\n",
                      fsm->state.object, *fsm->current);
            }
          break;

        case BUILD_VALUE:
          if (fsm->valueSuccess)
            {
              object_event (fsm, GOOD_VALUE);
            }
          else
            {
              object_event (fsm, OBJ_BV);
            }
          if (debug)
            {
              printf ("Current state (OBJECT): %d, Current char: %c\n",
                      fsm->state.object, *fsm->current);
            }
          break;

        case SCANNING:
          if (*fsm->current == ' ' || *fsm->current == '\n')
            {
              fsm->current++;
              object_event (fsm, WHITESPACE);
            }
          else if (*fsm->current == ',')
            {
              object_event (fsm, COMMA);
              fsm->current++;
            }
          else if (*fsm->current == '}')
            {
              object_event (fsm, CLOSE_CB);
            }
          else
            {
              object_event (fsm, BAD_TOKEN);
            }
          if (debug)
            {
              printf ("Current state (OBJECT): %d, Current char: %c\n",
                      fsm->state.object, *fsm->current);
            }
          break;

        default:
          break;
        }
    }

  // Run if successfully finished.
  if (fsm->state.object == OBJ_FINISH)
    {
      // Null terminate
      fsm->fullKeyString[fsm->fullKeyStringCurrentLocation] = '\0';
      *keys = fsm->fullKeyString;

      // Free data
      if (fsm->string_buffer != NULL)
        {
          if (debug)
            {
              printf ("Freeing string_buffer at %p\n",
                      (void *)fsm->string_buffer);
            }
          free (fsm->string_buffer);
        }

      if (fsm->identifierString != NULL)
        {
          if (debug)
            {
              printf ("Freeing identifierString at %p\n",
                      (void *)fsm->identifierString);
            }
          free (fsm->identifierString);
        }
      // Set empty to NULL
      fsm->fullKeyString = NULL;
      fsm->identifierString = NULL;
      fsm->string_buffer = NULL;
      return true;
    }
  return false;
}
