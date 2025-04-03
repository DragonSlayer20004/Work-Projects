#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "intmodel.h"
#include "objmodel.h"
#include "parser.h"
#include "statemodel.h"
#include "stringmodel.h"
#include "valmodel.h"

static bool get_args (int, char **, fsm_type_t *, char **);
static void usage (void);

int
main (int argc, char **argv)
{
  if (argc < 3)
    {
      usage ();
      return EXIT_FAILURE;
    }

  fsm_type_t type = INT;
  char *filename = NULL;

  if (!get_args (argc, argv, &type, &filename))
    {
      usage ();
      return EXIT_FAILURE;
    }

  char buffer[1024];

  // Open a file in read mode.
  int fd = open (filename, O_RDONLY);

  // Read file into fileString.
  int byteCount = read (fd, buffer, sizeof (char) * 1023);

  buffer[byteCount] = '\0';

  // Close the File.
  close (fd);

  fsm_t fsm;
  char *result = "";

  switch (type)
    {
    case STR:
      // Create the fsm.
      if (initialize_str_fsm (&fsm))
        {
          // Initialize input and current
          if (fd > 0)
            {
              fsm.input = buffer;
              fsm.current = buffer;
            }
          else
            {
              if (debug)
                {
                  printf ("Performing initialization without file.\n");
                }
              fsm.input = filename;
              fsm.current = filename;
            }

          // start the fsm.
          if (!accept_string (&fsm, &result))
            {
              printf ("Parsing %s failed\n", filename);
              return EXIT_FAILURE;
            }

          // Print result
          printf ("STRING: '%s'\n", result);
          if (result != NULL)
            {
              if (debug)
                {
                  printf ("Freeing result at %p\n", (void *)result);
                }
              free (result);
              result = NULL;
            }
        }
      break;

    case INT:
      // Create the fsm.
      if (initialize_int_fsm (&fsm))
        {
          // Initialize input and current
          if (fd > 0)
            {
              fsm.input = buffer;
              fsm.current = buffer;
            }
          else
            {
              if (debug)
                {
                  printf ("Performing initialization without file.\n");
                }
              fsm.input = filename;
              fsm.current = filename;
            }

          int64_t intResult;

          // start the fsm.
          if (!accept_integer (&fsm, &intResult))
            {
              printf ("Parsing %s failed\n", filename);
              return EXIT_FAILURE;
            }

          // Print result
          printf ("INTEGER: %ld\n", intResult);
        }
      break;

    case VAL:
      // Create the fsm.
      if (initialize_val_fsm (&fsm))
        {
          // Initialize input and current
          if (fd > 0)
            {
              fsm.input = buffer;
              fsm.current = buffer;
            }
          else
            {
              if (debug)
                {
                  printf ("Performing initialization without file.\n");
                }
              fsm.input = filename;
              fsm.current = filename;
            }

          bool isString;
          int64_t intResult;

          // start the fsm.
          if (!accept_value (&fsm, &isString, &result, &intResult))
            {
              printf ("Parsing %s failed\n", filename);
              return EXIT_FAILURE;
            }

          // Print result
          if (isString)
            {
              printf ("VALUE [string]: '%s'\n", result);
              if (result != NULL)
                {
                  if (debug)
                    {
                      printf ("Freeing result at %p\n", (void *)result);
                    }
                  free (result);
                  result = NULL;
                }
            }
          else
            {
              printf ("VALUE [integer]: %ld\n", intResult);
            }
        }
      break;

    case OBJ:
      // Create the fsm.
      if (initialize_obj_fsm (&fsm))
        {
          // Initialize input and current
          if (fd > 0)
            {
              fsm.input = buffer;
              fsm.current = buffer;
            }
          else
            {
              if (debug)
                {
                  printf ("Performing initialization without file.\n");
                }
              fsm.input = filename;
              fsm.current = filename;
            }

          // start the fsm.
          if (!accept_object (&fsm, &result))
            {
              printf ("Parsing %s failed\n", filename);
              return EXIT_FAILURE;
            }

          // Print result
          printf ("Key-value pairs:\n{\n%s}\n", result);
          if (result != NULL)
            {
              if (debug)
                {
                  printf ("Freeing result at %p\n", (void *)result);
                }
              free (result);
              result = NULL;
            }
        }
      break;

    default:
      break;
    }

  if (debug)
    {
      printf ("Success! Type: %d, File: %s\n", type, filename);
    }
  else
    {
      printf ("Success!\n");
    }

  return EXIT_SUCCESS;
}
/* Parse the command-line arguments. Sets the type based on whether the
   -i -s -v or -o flag was passed. If -d was passed, turn on debugging
   mode. Set the filename based on the last parameter. */
static bool
get_args (int argc, char **argv, fsm_type_t *type, char **filename)
{
  int ch = 0;
  while ((ch = getopt (argc, argv, "isvodh")) != -1)
    {
      switch (ch)
        {
        case 'i':
          *type = INT;
          break;
        case 's':
          *type = STR;
          break;
        case 'v':
          *type = VAL;
          break;
        case 'o':
          *type = OBJ;
          break;
        case 'd':
          debug = true;
          break;
        default:
          return false;
        }
    }
  *filename = argv[optind];
  return true;
}

static void
usage (void)
{
  printf ("json, a simple JSON parser\n\n");
  printf ("usage: json flag filename\n");
  printf ("filename is the name of the file with JSON contents.\n");
  printf ("flag is one of the following:\n");
  printf ("  -i         read the file as an integer\n");
  printf ("  -s         read the file as a string\n");
  printf ("  -v         read the file as a value (integer or string)\n");
  printf ("  -o         read the file as an object\n");
  printf ("  -d         turn on debugging mode\n");
  printf ("The last of -i, -s, -v, and -o will be used.\n");
}
