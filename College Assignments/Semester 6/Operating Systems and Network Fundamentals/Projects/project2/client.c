#include <assert.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "client.h"
#include "server.h"

/* Opens two message queues (mqreq and mqresp) to communicate a request
   for information about a file (filename) from the server.  The response
   from the server should be returned using the call-by-reference
   parameter (response). Returns true if successful, false otherwise. If
   any errors occur, be sure to close any opened message queues before
   returning. Note that the server requires any request received to have
   the type field of the ids_req_t struct to be set to REQUEST. The
   response from the server will have the ids_resp_t type field set to
   RESPONSE.
 */
bool
get_record (char *filename, char *mqreq, char *mqresp, ids_resp_t **response)
{
  // Check for NULL parameters to prevent segmentation faults.
  if (!filename || !mqreq || !mqresp || !response)
    {
      fprintf (stderr, "Error: NULL parameter passed to get_record()\n");
      return false;
    }

  // Send a request from the message queue.
  // Open the request.
  mqd_t mqdreq = mq_open (mqreq, O_WRONLY);
  if (mqdreq == -1)
    {
      fprintf (stderr, "mq_open request queue failed to open: %s\n", mqreq);
      return false;
    }

  // Generate the request.
  ids_req_t request;
  request.type = REQUEST;
  strncpy (request.filename, filename, sizeof (request.filename) - 1);
  request.filename[sizeof (request.filename) - 1] = '\0';

  // Send the request.
  // Cast the request to be a const char pointer
  if (mq_send (mqdreq, (const char *)&request, sizeof (request), 1) == -1)
    {
      perror ("mq_send failed");
      mq_close (mqdreq);
      return false;
    }

  // Close the request queue.
  if (mq_close (mqdreq) == -1)
    {
      perror ("mq_close request queue failed");
      return false;
    }

  // Reading in from from the response.
  // Open the response.
  mqd_t mqdresp = mq_open (mqresp, O_RDONLY);
  if (mqdresp == -1)
    {
      perror ("mq_open response queue failed");
      return false;
    }

  // Get attributes of the response.
  struct mq_attr attribute;
  if (mq_getattr (mqdresp, &attribute) == -1)
    {
      perror ("mq_getattr failed");
      mq_close (mqdresp);
      return false;
    }

  // Check if the message size is valid.
  if (attribute.mq_msgsize <= 0)
    {
      fprintf (stderr, "Error: Invalid message queue size: %ld\n",
               attribute.mq_msgsize); // perror() can't print parameters.
      mq_close (mqdresp);
      return false;
    }

  // Using response parameter and allocating space.
  *response = malloc (attribute.mq_msgsize);
  if (*response == NULL)
    {
      perror ("malloc failed");
      mq_close (mqdresp);
      return false;
    }

  // Receive the response.
  unsigned int priority = 0;
  ssize_t bytes_received = mq_receive (mqdresp, (char *)*response,
                                       attribute.mq_msgsize, &priority);
  if (bytes_received == -1)
    {
      perror ("mq_receive failed");
      free (*response); // Free allocated memory to avoid leaks.
      mq_close (mqdresp);
      return false;
    }

  // Close the response queue.
  if (mq_close (mqdresp) == -1)
    {
      perror ("mq_close response queue failed");
      free (*response);
      return false;
    }

  if ((*response)->type != RESPONSE)
    {
      perror ("response type incorrect");
      free (*response);
      *response = NULL;
      return false;
    }

  return true;
}

/* Given an ids_resp_t struct from the server for the specified file
   (filename), confirm that the fields are correct. You will need to use
   fstat() and execute the cksum program as in Assignment 4 to get the local
   information. Return true if all fields are correct, and false otherwise.
 */
bool
check_record (char *filename, ids_resp_t *response)
{
  int fd = open (filename, O_RDONLY);

  if (fd == -1)
    {
      printf ("ERROR: Failed to get record for %s\n", filename);
      return false;
    }

  if (response == NULL)
    {
      perror ("Response is NULL.\n");
      return false;
    }

  // Get fstat of open file.
  struct stat fileStat;

  if (fstat (fd, &fileStat) == -1)
    {
      perror ("Failed to assign fileStat.\n");
      close (fd);
      return false;
    }

  // Copy useful information into response.
  mode_t realMode = fileStat.st_mode;
  off_t realSize = fileStat.st_size;

  // Close the file before opening it again in get_cksum.
  close (fd);

  // Get the checksum value.
  // Use strncpy for fixed-sized buffers
  char *checkSum = get_cksum (filename);
  // Split the string to get rid of the space.
  split_string (checkSum);

  // Use the following print format strings for correct results.
  printf ("%s:\n", filename);

  bool inaccurate = false;

  if (realMode != response->mode)
    {
      printf ("  permissions: %o [MISMATCH %o]\n", response->mode, realMode);
      inaccurate = true;
    }
  else
    {
      printf ("  permissions: %o\n", realMode);
    }

  if (realSize != response->size)
    {
      printf ("  size: %zu [MISMATCH %zu]\n", response->size, realSize);
      inaccurate = true;
    }
  else
    {
      printf ("  size: %zu\n", realSize);
    }

  if (strcmp (checkSum, response->cksum) != 0)
    {
      printf ("  cksum: %s [MISMATCH %s]\n", response->cksum, checkSum);
      inaccurate = true;
    }
  else
    {
      printf ("  cksum: %s\n", checkSum);
    }

  // If there is a mismatch, include " [MISMATCH X]" before the newline,
  // where X is the value you retrieve locally. For instance, if the size
  // is wrong, replace the printed message with:
  // printf ("  size: %zu [MISMATCH: %zu]\n", server_size, my_size);

  free (checkSum);
  checkSum = NULL;

  // Check if check the records are inaccurate.
  if (inaccurate)
    {
      return false;
    }
  else
    {
      return true;
    }
}
/* Given a writable string buffer, find the first space and replace it
   with a '\0', thus ending the string at that location. Sample call:

     char buffer[] = "hello world";
     // result points to buffer, which prints as "hello"
     char *result = split_string (buffer);
*/
char *
split_string (char *buffer)
{
  if (buffer == NULL)
    {
      return NULL;
    }

  for (int i = 0; buffer[i] != '\0'; i++)
    {
      if (buffer[i] == ' ' || buffer[i] == '\n')
        {
          buffer[i] = '\0'; // Replace space with null terminator
          return buffer;
        }
    }
  return buffer;
}

/* Creates a child process. If this fails, close the pipe and return NULL.
   If the child is created, use it to run the cksum program on the filename
   passed. Send the result back through the pipe. The parent process should
   just return the child's PID.
*/
pid_t
create_cksum_child (int *pipe, const char *filename)
{
  // Create child
  pid_t child_pid = fork ();

  if (child_pid < 0)
    { // Fork Failed
      printf ("Fork Failed\n");
      close (pipe[0]);
      close (pipe[1]);
      return EXIT_FAILURE;
    }

  if (child_pid == 0)
    {                  // Child process
      close (pipe[0]); // Close unused read end

      // Redirect stdout to the write end of the pipe
      dup2 (pipe[1], STDOUT_FILENO);
      close (pipe[1]); // Close original write end

      // execlp replaces the current running process with a new process. In
      // this case we are replacing it with the checksum process Execute
      // `cksum` on the filename
      execlp ("cksum", "cksum", filename, NULL);

      // Should never get here as it should be in a new process.
      // Only if execlp fails will you get here.
      printf ("execlp failed\n");
      exit (EXIT_FAILURE);
    }

  close (pipe[1]); // Parent does not need to write.
  return child_pid;
}

/* Uses the cksum program on the input filename. Start by creating a pipe
   and calling create_cksum_child() to create the child process. If that
   fails, return NULL. Otherwise, Implement the parent here so that it
   reads the result of the cksum calculation from the pipe and returns
   the result.
*/
char *
get_cksum (const char *filename)
{
  int fd[2];
  if (pipe (fd) == -1)
    { // Failed to turn fd into a pipe
      printf ("pipe failed\n");
      return NULL;
    }

  pid_t child_pid = create_cksum_child (fd, filename);

  if (child_pid < 0)
    { // Failed to make child
      printf ("fork failed\n");
      close (fd[0]);
      close (fd[1]);
      return NULL;
    }

  close (fd[1]); // Parent does not need to write.

  waitpid (child_pid, NULL, 0); // wait for child

  // Read in child
  char *checkSumString = malloc (sizeof (char) * 1024);
  size_t bytes = read (fd[0], checkSumString, 1023);
  checkSumString[bytes] = '\0';
  close (fd[0]);

  return checkSumString;
}