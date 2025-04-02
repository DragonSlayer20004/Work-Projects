#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "server.h"

/* Creates a new process running the server program. The server takes
   two required argument (mqreq and mqresp) and one optional (pidfile).
   The required arguments indicate the names of the message queues to
   use. The optional pidfile specifies the name of a file to store the
   server's PID in. The server uses this file to detect if it is already
   running. You can also look in this file to determine the PID if you
   need to kill it using the kill utility. Run ./server -h on the command
   line to see the required format of the command-line arguments.

   CRITICAL: It is important to get the timing of this function correct.
   This function will create the process and execute the server program;
   it must wait long enough to ensure the server starts successfully, but
   it cannot wait for the server to exit (which would defeat the purpose
   of a server!). Instead, this function must redirect the server's
   STDOUT to a pipe for reading from. The server will write a message
   containing "SUCCESS" once it is ready. If the server encountered an
   error and could not start, it will print an "ERROR" message. This
   function should wait until it receives one of these messages, then
   return.
 */
bool
start_server (char *pidfile, char *mqreq, char *mqresp)
{

  // Create a pipe
  int fd[2];
  if (pipe (fd) == -1)
    { // Failed to turn fd into a pipe
      printf ("pipe failed\n");
      return NULL;
    }

  // Create child
  pid_t child_pid = fork ();
  if (child_pid < 0)
    { // Fork Failed
      printf ("Fork Failed\n");
      close (fd[0]);
      close (fd[1]);
      return EXIT_FAILURE;
    }

  // Child function
  if (child_pid == 0)
    {                // Child process
      close (fd[0]); // Close unused read end

      // Redirect stdout to the write end of the pipe
      dup2 (fd[1], STDOUT_FILENO);

      // execlp replaces the current running process with a new process. In
      // this case we are replacing it with the server process Execute `server`
      // on the pidfile
      execlp ("./server", "./server", "-p", pidfile, mqreq, mqresp, NULL);

      // Should never get here as it should be in a new process.
      // Only if execlp fails will you get here.
      printf ("execlp failed\n");
      exit (EXIT_FAILURE);
    }

  // Parent function
  close (fd[1]); // Parent does not need to write

  // Read in from the child.
  char childResponse[1024];
  size_t bytes = 0;
  while ((bytes = read (fd[0], childResponse, 1023)) > 0)
    {
      childResponse[bytes] = '\0';

      if (strstr (childResponse, "SUCCESS") != NULL)
        {
          close (fd[0]);
          return true;
        }
      else if (strstr (childResponse, "ERROR") != NULL)
        {
          close (fd[0]);
          return false;
        }
    }
  close (fd[0]); // Done reading
  return true;
}

/* Stops the server. Opens the file (pidfile) that contains the server's
   PID as an int. After reading that value into a variable, sends the
   SIGUSR1 signal to the server. The server contains a signal handler that
   will cleanly shut down (deleting the message queues and the pidfile in
   the process) when it receives this signal. Returns true if the server
   has shut down (by detecting the deletion of pidfile), false otherwise.

   Note that there may be a time delay between sending the signal and when
   the server deletes the file. After sending the SIGUSR1 signal, this
   function should check for the file to be deleted up to 5 times (waiting
   for 1 second between each check). Return false only if the file is still
   around after the fifth check.
 */
bool
stop_server (char *pidfile)
{
  // Open PID file
  // Send SIGUSR1 to PID specified in it
  int fd = open (pidfile, O_RDONLY);
  if (fd == -1)
    {
      perror ("Failed to open PID file.");
      return false;
    }

  // Get the PID from the file.
  char pidBuffer[16];
  int bytesRead = read (fd, pidBuffer, 15);
  close (fd);

  if (bytesRead <= 0)
    {
      perror ("Failed to read in PID.");
      return false;
    }
  pidBuffer[bytesRead] = '\0';

  // Read the buffer into pid.
  pid_t pid = atoi (pidBuffer);
  if (pid <= 0)
    {
      perror ("Invalid PID read from file.");
      return false;
    }

  // Send SIGUSR1 to the process
  if (kill (pid, SIGUSR1) == -1)
    {
      perror ("Failed to send SIGUSR1.");
      return false;
    }

  // Try up to 5 times of checking for the file deletion
  for (int i = 0; i < 5; i++)
    {
      if (access (pidfile, F_OK) == -1)
        {
          return true; // File deleted
        }

      sleep (1);
    }

  perror ("Server did not terminate in time.");
  return false;
}
