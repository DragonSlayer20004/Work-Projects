#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "client.h"
#include "ids.h"
#include "server.h"
#include "support.h"

static bool get_args (int, char **, char **, char **, int *, bool *, bool *);
static void usage ();

// Enable debugging tooltips.
bool debug = false;

int
main (int argc, char **argv)
{
  /* DEBUGGING CODE to test stop and start server
  if (start_server("PID","/comp3400.int.req", "/comp3400.int.resp" ))
  {
          printf("Server started\n");
          }
  else{
          printf("Server not started\n");
  }
  return 0;
  */
  char *pidfile = NULL;      // server process's PID file
  char *outfile = NULL;      // server output file
  int index = -1;            // index of first file to query
  bool killServer = false;   // determine whether or not we need to kill the
                             // server at the end.
  bool queryResults = false; // determine whether or not we need to send output
                             // to a new file.

  if (!get_args (argc, argv, &pidfile, &outfile, &index, &killServer,
                 &queryResults))
    {
      usage ();
      return EXIT_FAILURE;
    }

  if (index >= argc - 2)
    {
      usage ();
      return EXIT_FAILURE;
    }

  char *mqreq = argv[index++];  // name of the request message queue
  char *mqresp = argv[index++]; // name of the response message queue

  if (pidfile == NULL)
    {
      pidfile = "PID";
    }

  if (access (pidfile, F_OK) == -1)
    {
      start_server (pidfile, mqreq, mqresp);
    }

  // Mapping variables if querying is true
  char *mapped = NULL;
  ids_entry_t *container = NULL;
  int entryCount = 0;

  // Get the record from the server.
  while (index < argc)
    {
      char *fileName = argv[index++]; // name of the file we are reading in

      // Create the response variable.
      ids_resp_t *response = NULL;

      if (!(get_record (fileName, mqreq, mqresp, &response)))
        {
          perror ("get_record failed\n");
        }
      else
        {
          // Save to container if we are querying the results.
          if (queryResults)
            {
              // realloc to adjust for increased size.
              container = realloc (container,
                                   sizeof (ids_entry_t) * (entryCount + 1));
              if (container == NULL)
                {
                  perror ("Failed to allocate memory for container.");
                  return EXIT_FAILURE;
                }

              if (response != NULL)
                {
                  strcpy (container[entryCount].cksum, response->cksum);
                  strcpy (container[entryCount].filename, fileName);
                  container[entryCount].mode = response->mode;
                  container[entryCount].size = response->size;
                  container[entryCount].valid = response->type;
                }
              // Increment entryCount for every response.
              entryCount++;
            }
        }

      check_record (fileName, response);

      // Free allocated memory.
      if (response != NULL)
        {
          if (debug)
            {
              printf ("Freeing response after success.\n");
            }
          free (response);
        }
    }

  if (killServer)
    {
      if (stop_server (pidfile) == false)
        {
          perror ("Failed to close the server.");
        }
    }

  // If we are querying the results to create a mmap and map the results into
  // it.
  if (queryResults)
    {
      int fd; // Only writing so we don't need to open the read end and only
              // need it to be a single int.
      size_t containerSize = 0;

      // Open the outfile file.
      // O_Create is neccissary to create a new file. HOWEVER it cannot create
      // folders. outputs/OUTFILE doesn't work if the folder outputs doesn't
      // exist.
      fd = open (outfile, O_RDWR | O_CREAT, 0666);
      if (fd == -1)
        {
          fprintf (stderr, "Error opening outFile: %s\n", outfile);
          close (fd);
          return EXIT_FAILURE;
        }

      // Get containerSize.
      containerSize = sizeof (ids_entry_t) * entryCount;
      if (ftruncate (fd, containerSize) == -1)
        { // Use ftruncate to get the exact size of the file.
          perror ("Error truncating file"); // ftruncate returns 0 on success
                                            // and -1 on failure.
          close (fd);
          return EXIT_FAILURE;
        }

      // Check to make sure containerSize was set correctly.
      if (containerSize == 0)
        {
          perror ("Error: containerSize is 0. Cannot map an empty file.\n");
          close (fd);
          return EXIT_FAILURE;
        }

      mapped = mmap (NULL, containerSize, PROT_READ | PROT_WRITE, MAP_SHARED,
                     fd, 0);
      if (mapped == MAP_FAILED)
        {
          perror ("Error mapping file");
          close (fd);
          return EXIT_FAILURE;
        }

      // Check if container is valid before writing.
      if (container == NULL)
        {
          fprintf (stderr, "Error: No valid data to write.\n");
          munmap (mapped, containerSize);
          close (fd);
          return EXIT_FAILURE;
        }

      // Write data into the mmap.
      memcpy (mapped, container,
              containerSize); // Copy a block of data into mapped location.

      // Cleanup: Unmap and close file
      if (munmap (mapped, containerSize) == -1)
        {
          perror ("Error unmapping file");
        }
      close (fd);
      free (container);
    }

  return EXIT_SUCCESS;
}

/* Parse the command-line arguments. */
static bool
get_args (int argc, char **argv, char **pidfile, char **outfile, int *index,
          bool *killServer, bool *queryResults)
{
  int ch = 0; // : in getopt represents it has a parameter.
  while ((ch = getopt (argc, argv, "p:hko:"))
         != -1) // Added k to allow for killing of the server.
    {
      switch (ch)
        {
        case 'p':
          *pidfile = optarg;
          break;
        case 'k': // Added k to allow for killing of the server.
          *killServer = true;
          break;
        case 'o':
          *queryResults = true;
          *outfile = optarg;
          break;
        default:
          return false;
        }
    }
  *index = optind;
  return true;
}

static void
usage (void)
{
  printf ("ids, a message-queue based intrusion-detection system\n\n");
  printf ("usage: ids [options] mqreq mqresp file1 ...\n");
  printf ("file1 is a file on disk to query from the server to get\n");
  printf ("the file info from the last time it was checked.\n\n");
  printf ("mqreq and mqresp are used to identify the message queues\n");
  printf ("to communicate with the server.\n\n");
  printf ("options can be one or more of the following:\n");
  printf ("  -k          Kill the server after performing the queries\n");
  printf ("  -o outfile  Write the query results to the file \"outfile\"\n");
  printf ("  -p pidfile  Use \"pidfile\" to store the server's PID");
  printf (" (default \"PID\")\n");
}
