/*
 * COMP 3400: Template lab driver
 *
 * Name: Kyle Hinkle
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "statemodel.h"

int ConvertToInt (const char *str);

void
usage (void)
{
  printf ("fsm, a state model simulator\n\n");
  printf ("usage: fsm [evt1 evt2 ...]\n");
  printf (
      "evt1 evt2 ... is a sequence of event combinations. Each event is\n");
  printf ("represented by an int value:\n\n");
  printf ("  ADMIT = 0\n");
  printf ("  DISPATCH = 1\n");
  printf ("  SCHEDULE = 2\n");
  printf ("  BLOCK = 3\n");
  printf ("  UNBLOCK = 4\n");
  printf ("  EXIT = 5\n");
  printf ("  KILL = 6\n");
  printf ("  NIL = 7\n");
}

int
main (int argc, char **argv)
{
  // With no arguments, show usage and exit
  if (argc < 2)
    {
      usage ();
      return EXIT_FAILURE;
    }

  // Declare and initialize a FSM
  fsm_t process;
  if (!initialize_fsm (&process))
    return EXIT_FAILURE;

  // EXAMPLE: Send ADMIT and DISPATCH to the process.
  // TODO: Replace this with a loop for all events.

  // Start at 1 to skip command name.
  for (int i = 1; i < argc; i++)
    {
      int event = ConvertToInt (argv[i]);
      if (event == -1)
        {
          fprintf (stderr, "Error: Invalid event '%s'\n", argv[i]);
          return EXIT_FAILURE;
        }
      handle_event (&process, event);
    }
  // Under normal runs, we should always end in TRM state
  if (process.state != TRM)
    {
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int
ConvertToInt (const char *str)
{
  static const char *event_names[]
      = { "ADMIT",   "DISPATCH", "SCHEDULE", "BLOCK",
          "UNBLOCK", "EXIT",     "KILL",     "NIL" };

  // Check if input is a valid integer within the range 1-8
  int num = atoi (str);
  if (num >= 0 && num < 9)
    {
      return num;
    }

  for (int i = ADMIT; i <= NIL; i++)
    {
      if (strcmp (str, event_names[i]) == 0)
        {
          return i;
        }
    }
  return -1;
}
