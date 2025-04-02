#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pipe.h"

/* Given a writable string buffer, find the first space and replace it
   with a '\0', thus ending the string at that location. Sample call:

     char buffer[] = "hello world";
     // result points to buffer, which prints as "hello"
     char *result = split_string (buffer);
*/
char *
split_string (char *buffer)
{
  if (buffer == NULL) {
      return NULL;
  }

  for (int i = 0; buffer[i] != '\0'; i++) {
      if (buffer[i] == ' ' || buffer[i] == '\n') {
          buffer[i] = '\0';  // Replace space with null terminator
          return buffer;
      }
  }
  return buffer;
}

/* Creates a child process. If this fails, close the pipe and return NULL.
   If the child is created, use it to run the cksum program on the filename
   passed. Send the result back through the pipe. The parent process should
   just return the child's PID. Sample call:

     int fd[2];
     pipe (fd);
     pid_t child_pid = create_cksum_child (fd, "foo.txt");
*/
pid_t
create_cksum_child (int *pipe, const char *filename)
{
  //Create child
  pid_t child_pid = fork();

  if(child_pid < 0){ //Fork Failed
    printf("Fork Failed\n");
    close(pipe[0]);
    close(pipe[1]);
    return EXIT_FAILURE;
  }

  if (child_pid == 0) {  // Child process
      close(pipe[0]);  // Close unused read end

      // Redirect stdout to the write end of the pipe
      dup2(pipe[1], STDOUT_FILENO);
      close(pipe[1]);  // Close original write end

      // execlp replaces the current running process with a new process. In this case we are replacing it with the checksum process
      // Execute `cksum` on the filename
      execlp("cksum", "cksum", filename, NULL);

      // Should never get here as it should be in a new process. 
      // Only if execlp fails will you get here.
      printf("execlp failed\n");
      exit(EXIT_FAILURE);
  }

  close(pipe[1]); //Parent does not need to write.
  return child_pid;
}

/* Uses the cksum program on the input filename. Start by creating a pipe
   and calling create_cksum_child() to create the child process. If that
   fails, return NULL. Otherwise, Implement the parent here so that it
   reads the result of the cksum calculation from the pipe and returns
   the result. Sample call:

     // sum is "3015617425 6 data/f1.txt\n" [with the newline]
     char *sum = get_cksum ("data/f1.txt");
*/
char *
get_cksum (const char *filename)
{
  int fd[2];
  if(pipe (fd) == -1){ //Failed to turn fd into a pipe
    printf("pipe failed\n");
    return NULL;
  } 

  pid_t child_pid = create_cksum_child (fd, filename);

  if(child_pid < 0){ //Failed to make child
    printf("fork failed\n");
    close(fd[0]);
    close(fd[1]);
    return NULL;
  }

  close(fd[1]); //Parent does not need to write.

  waitpid(child_pid, NULL, 0); // wait for child

  // Read in child
  char *checkSumString = malloc(sizeof(char) * 1024);
  size_t bytes = read(fd[0], checkSumString, 1023);
  checkSumString[bytes] = '\0';
  close(fd[0]);

  return checkSumString;
}

/* Opens two FIFOs, one for reading (O_RDONLY) and one for writing
   (O_WRONLY). 
   Reads in the name of a file from the reading FIFO, then
   gets the cksum output from get_cksum(). 
   Uses split_string() to get only the first token (full output would be something like
   "12345678   25   filename.txt"). Writes the cksum value (12345678)
   into the writing FIFO and returns that string. Make sure to close
   both FIFOs before returning. Sample call:

     // if received "data/f1.txt" from fifo.in,
     // writes "3015617425" to fifo.out and returns "3015617425"
     char *sum = fifo_server ("fifo.in", "fifo.out");
*/
char *  
fifo_server (char *fifo_in, char *fifo_out)
{
  //Open FIFO for reading and writing
  int fifoRead = open(fifo_in, O_RDONLY);
  if (fifoRead == -1) {
    printf("Error opening file\n");
    close(fifoRead);
    unlink(fifo_in);
    unlink(fifo_out);
    return NULL;
  }
  
  int fifoWrite = open(fifo_out, O_WRONLY);
  if (fifoWrite == -1) {
    printf("Error opening file\n");
    close(fifoRead);
    close(fifoWrite);
    unlink(fifo_in);
    unlink(fifo_out);
    return NULL;
  }

  //Read in from FIFO
  char newWord[1024];
  // Read from input. 100 = 100 chars
  ssize_t bytes = read(fifoRead, newWord, 1023);
  newWord[bytes] = '\0';

  //Get checksum and split
  char *checksum = get_cksum(newWord);
  split_string(checksum);
  //End of reading from the FIFO

  // Write checksum to output
  write(fifoWrite, checksum, strlen(checksum));

  close(fifoRead);
  close(fifoWrite);
  unlink(fifo_in);
  unlink(fifo_out);
  return checksum;
}
