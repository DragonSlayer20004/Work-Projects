#include <assert.h>
#include <fcntl.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "pipe.h"

#define LINE_LENGTH 12

/* Uses fstat() to get the file size for the provided file descriptor.
   Sample call:

     int fd = open ("data/f1.txt");
     size_t size = get_file_size (fd); // returns 6
*/
size_t
get_file_size (int fd)
{
  // Create stat struct bassed on the file descriptor.
  struct stat statistics;
  fstat(fd, &statistics);

  // Aquire size from stat struct.
  size_t fileSize = statistics.st_size;

  return fileSize;
}

/* Opens the index file based on the provided name. Returns a pointer to
   the memory map of the file. Also sets the call-by-reference parameters
   fd (file descriptor for the open file) and size (the file size in bytes).
   Uses get_file_size() as a helper. Return NULL if anything fails, closing
   the file if needed. Sample call:

     int fd;
     size_t size;
     // map contains the contents of data/index, fd is set to the
     // opened file descriptor, and size is 36 (number of bytes in data/index)
     char *map = open_index ("data/index", &fd, &size);
*/
char *
open_index (const char *index, int *fd, size_t *size)
{
  assert (fd != NULL);
  assert (size != NULL);

  // Open the index file.
  *fd = open(index, O_RDWR);
  if (*fd == -1) {
      printf("Error opening file\n");
      close(*fd);
      return NULL;
  }

  // Get size.
  *size = get_file_size(*fd);

  // Make the memory map. Allow read and write permissions. Enable the map to be editable for all. Offset 0 for start of file.
  char *mapped = mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
  if(mapped == MAP_FAILED){
    printf("Error mapping file\n");
    close(*fd);
    return NULL;
  }

  return mapped;
}

/* Returns a pointer to the beginning of line number lineno. First check
   to make sure that the line number is valid (the position would not be
   beyond the filesize). Note that in this lab, all lines of the file are
   LINE_LENGTH characters long. Hence, line number i begins at offset
   i * LINE_LENGTH in the memory map. Sample call:

     // text_line points to "data/f2.txt" in the mapped file
     char *text_line = get_file_name (map, 1, 36);
*/
char *
get_file_name (char *map, size_t lineno, size_t filesize)
{
  if(lineno * LINE_LENGTH > filesize){
    printf("Out of bounds request\n");
  }

  return &map[lineno * LINE_LENGTH];
}

/* Re-implementation of the create_cksum_child() in pipe.c. Instead of
   using the calls to pipe/fork/dup2/exec, combine the latter three
   into a call to posix_spawn(). Sample call:

     // sum is "3015617425 6 data/f1.txt\n" [with the newline]
     char *sum = spawn_cksum ("data/f1.txt");
*/
char *
spawn_cksum (char *filename)
{
  // Instead of using fork() and exec(), use the posix_spawn functions.
  // Note that you will need to add a close and dup2 action to the
  // set of file actions to run when spawning.

  // Create a struct of actions
  posix_spawn_file_actions_t actions;

  //Create the child
  pid_t child;
  int fd[2];

  // Use these parameters to posix_spawn(). You will need to set up
  // the posix_spawn_file_actions_t by 
  // (1) initializing it, 
  // (2) adding
  // a close action, and 
  // (3) adding a dup2 action so that STDOUT writes
  // to a pipe. 
  // After spawning the new process, make sure to call
  // posix_spawn_file_actions_destroy() on the actions to prevent
  // memory leaks.

  const char *path = "/usr/bin/cksum";
  char *const argv[] = { "cksum", filename, NULL };

  // Open and create mmap.
  //char *map = open_index(filename, &fd, &size);

  // Make pipe.
  if(pipe (fd) == -1){ //Failed to turn fd into a pipe
    printf("pipe failed\n");
    return NULL;
  }

  // DO NOT need to open pipe since posix does that for us.

  // Initialize file actions.
  posix_spawn_file_actions_init(&actions);

  // Adding a close action. Child does not need to read
  posix_spawn_file_actions_addclose(&actions, fd[0]);

  // Adding dup2 action to rerout STDOUT to the pipe. fd[1] becomes STDOUT_FILENO
  posix_spawn_file_actions_adddup2(&actions, fd[1], STDOUT_FILENO);

  // Close original write end now that we are writing into STDOUT
  posix_spawn_file_actions_addclose(&actions, fd[1]);

  // All parameters have been constructed. Now spawn the child.
  // The posix_spawn will create a child and have it change it's process to be cksum.
  // Similar to how we did it in pipes but without have to fork and copy everything over first.
  // Argv is the new process that we want to run.
  if(posix_spawn(&child, path, &actions, NULL, argv, NULL) != 0){
    printf("Failed to make posix_spawn.\n");
    return NULL;
  }

  // Child will never make it down to here since it executed itself onto a new process. Only the parent.

  // Close writing as parent will never write.
  close(fd[1]);
  // Parent code: read the value back from the pipe into a dynamically
  // allocated buffer. Wait for the child to exit, then return the
  // buffer.

  //Read in child
  char *checkSumString = malloc(sizeof(char) * 1024);
  size_t bytes = read(fd[0], checkSumString, 1023);
  checkSumString[bytes] = '\0';
  close(fd[0]);

  // Wait for child to finish.
  waitpid(child, NULL, 0);

  // Destroy actions and cleanup once we are done.
  posix_spawn_file_actions_destroy(&actions);

  return checkSumString;
}
