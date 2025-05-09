#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "map.h"
#include "pipe.h"

char *build_name (char *);

int cmdline (int, char **, bool *, bool *, char **, size_t *);

void
usage (void)
{
  printf ("Usage: ipc [options] ARG\n");
  printf (" With neither flag, send ARG as a filename to a pipe.\n");
  printf (" Options are:\n");
  printf ("  -s      Create a server that listens using FIFOs.\n");
  printf ("          ARG must be the file name to send through the FIFO.\n");
  printf ("  -m      Use data/index as an index of file names to use as a\n");
  printf ("          memory map. ARG must be a line number in that file.\n");
}

int
main (int argc, char **argv)
{
  bool use_map = false;
  bool use_server = false;
  char *fname = NULL;
  size_t lineno = -1;

  if (cmdline (argc, argv, &use_map, &use_server, &fname, &lineno) < 0)
    {
      usage ();
      return EXIT_FAILURE;
    }

  if (!use_server && !use_map)
    {
      // FIRST STEP: Send the file name to get_cksum(), then split the
      // string. Print the following message:
      char *sum = get_cksum(argv[1]);
      split_string(sum);
      printf ("CKSUM(%s) = '%s'\n", fname, sum);
      // Free sum after printing it.
      free(sum);
    }
  else if (use_server)
    {
      char *fifo_cp = build_name ("cp"); // FIFO for child-to-parent communication
      char *fifo_pc = build_name ("pc"); // FIFO for parent-to-child communication

      // Create the FIFOs with read and write permissions
      // For mkfifo mode the 0 represents that its in octal and the 6 represents read write permissions. 666 represents read write permissions for everybody.
      if(mkfifo(fifo_pc, 0666) == -1){ //Read
        printf("Failed to open %s", fifo_pc);
      }
      if(mkfifo(fifo_cp, 0666) == -1){ //Write
        printf("Failed to open %s", fifo_cp);
      } 

      // Create the child and call fifo_server() in that process.
      pid_t child = fork ();
      assert (child >= 0);
      if (child == 0)
      {
        char *result = fifo_server (fifo_pc, fifo_cp);
        free (fifo_cp);
        free (fifo_pc);
        free (result);
        return EXIT_SUCCESS;
      }

      // Implement the parent here. Instead of calling get_cksum() as
      // before, it will send the file name into a FIFO that the child
      // is reading from. The child will get the cksum and send it back
      // through the FIFO, printing as before:

      // Write to the child
      int fifo = open (fifo_pc, O_WRONLY);

      write(fifo, argv[2], strlen(argv[2]));
      close(fifo);

      // Read from the child
      fifo = open (fifo_cp, O_RDONLY);

      char sum[1024];
      ssize_t bytes = read(fifo, sum, 1023);
      sum[bytes] = '\0';
      close(fifo); //Close once we have read it in.

      printf ("CKSUM(%s) = '%s'\n", fname, sum);

      // Don't forget to close the FIFOs before exiting.
      char buffer[BUFFER_LENGTH];
      memset (buffer, 0, sizeof (buffer));

      waitpid(child, NULL, 0);

      // Clean up the FIFOs before exiting
      unlink (fifo_cp);
      free (fifo_cp);
      unlink (fifo_pc);
      free (fifo_pc);

      return EXIT_SUCCESS;
    }
  else
    {
      // Map the index file into memory with open_index(). Call
      char *index = "data/index";
      int fd[2];
      size_t size = 0;
      char *mapped = open_index(index, fd, &size);

      // get_file_name() with the lineno command-line parameter to get
      // the file name.
      char *filePathName = get_file_name(mapped, atoi(argv[2]), size);

      // Send that name (filepath) to spawn_cksum() to get the value and
      // print it out.
      char *sum = spawn_cksum (filePathName);

      // Split string before printing.
      split_string(sum);

      printf ("CKSUM(%s) = '%s'\n", filePathName, sum);
      free(sum);
      return EXIT_SUCCESS;
    }
}

/* Helper for constructing a FIFO name. The returned string will
   be something like "/tmp/pc.dbowie" (indicating parent-to-child for
   the user dbowie). This string must be freed later. */
char *
build_name (char *direction)
{
  char *fifo = calloc (BUFFER_LENGTH, sizeof (char));
  snprintf (fifo, BUFFER_LENGTH, "/tmp/");
  strncat (fifo, direction, BUFFER_LENGTH - strlen (fifo));
  strncat (fifo, ".", BUFFER_LENGTH - strlen (fifo));
  char *fifoname = getlogin ();
  assert (fifoname != NULL);
  strncat (fifo, fifoname, BUFFER_LENGTH - strlen (fifo));
  unlink (fifo);
  return fifo;
}

/* Helper for parsing the command line arguments. Sets map, server,
   filename, and lineno as call-by-reference parameters. Return 0
   on success, -1 otherwise. */
int
cmdline (int argc, char **argv, bool *map, bool *server, char **filename,
         size_t *lineno)
{
  int option;

  while ((option = getopt (argc, argv, "smh")) != -1)
    {
      switch (option)
        {
        case 's':
          if (*map)
            return -1;
          *server = true;
          break;
        case 'm':
          if (*server)
            return -1;
          *map = true;
          break;
        case 'h':
          return -1;
          break;
        default:
          return -1;
        }
    }

  if (optind >= argc)
    return -1;

  if (*map)
    {
      *lineno = (size_t)strtol (argv[optind], NULL, 10);
      if (*lineno < 0 || *lineno > 2)
        return -1;
    }
  else
    *filename = argv[optind];

  return 0;
}
