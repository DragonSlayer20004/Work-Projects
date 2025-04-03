#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils.h"

/* Basic framework for a web server than can handle a single request, then
   shuts down. Looks for files in the srv_root directory.
 */
char *
serve_web (char *protocol)
{
  // Pass the protocol (should be a port number) to
  // setup_server() to get the socket.
  int socketfd = setup_server (protocol);
  if (socketfd < 0)
    return NULL;

  // Indicate (for debugging) that the server is running
  fprintf (stderr, "Server is started\n");

  int connection = -1;
  char *address = get_connection (socketfd, &connection);
  if (address == NULL)
    return NULL;
  
  // Format the message so we can write to the connection
  char message[100];
  snprintf(message, sizeof(message), "Received incoming request from %s\n", address);

  // Write debug message
  printf("%s", message);

  // TODO: Read the request from the socket into the buffer provided above.
  // If the call to read() fails, shutdown and close both file descriptors
  // and return NULL.
  char buffer[BUFFER_SIZE];
  memset (&buffer, 0, BUFFER_SIZE);

  int result = read(connection, buffer, BUFFER_SIZE);
  if(result < 0){
    close(socketfd);
    close(connection);
    return NULL;
  }

  // TODO: Look at the first line of the request to get the URI and the
  // HTTP version. For MIN: anything other than a GET request for
  // "/index.html" should be considered a non-existent file that will
  // later cause a 404 Not Found. 
  
  // For FULL: allow the URI to be "/" (which
  // you will convert to "/index.html") or "/bootstrap.html". HINT: You
  // can use strtok() to tokenize the line read (i.e., break it into parts).
  // Specifically, the first call to strtok (buffer, " "); would return a
  // pointer to "GET". Then, calling it as strtok (NULL, " "); would return
  // a pointer to the URI. For the third call, replace the second argument
  // with "\r", as you want to split the buffer at the carriage return.

  // Extract method, URI, and HTTP version
  // Assuming GET /index.html HTTP/1.1 use strtok deliminating on empty space.
  // This will get the GET method.
  char *method = strtok(buffer, " ");
  // This will get the URI aka index.html or / that the client wants.
  char *uri = strtok(NULL, " ");
  // This will get the http version.
  char *version = strtok(NULL, "\r");

  //If we fail to receive one of the above or a method that isn't a GET return NULL.
  if (!method || !uri || !version || strcmp(method, "GET") != 0) {
      write(connection, "HTTP/1.0 400 Bad Request\r\n\r\n", 28);
      close(socketfd);
      close(connection);
      return NULL;
  }

  // Change directory if URI is /
  if(strcmp(uri, "/") == 0){
    uri = "/index.html";
  }

  // Format the message so we can write to the connection
  snprintf(message, sizeof(message), "GET Request for %s using %s\n", uri, version);

  // Write debug message
  printf("%s", message);

  // Prepend "srv_root" to the beginning of the URI to get the full
  // URI location, such as srv_root/index.html.
  char *uri_loc = strdup ("srv_root");
  size_t length = strlen (uri_loc) + strlen (uri);
  uri_loc = realloc (uri_loc, length + 1);
  strncat (uri_loc, uri, length - strlen (uri_loc));

  char *contents = NULL;
  char *header = build_response (uri_loc, version, &contents);
  // TODO: If the value returned from build_response() is not NULL, then
  // write the header and contents into the socket. If the returned header
  // is NULL, send a 404 based on the HTTP version (HTTP/1.1 only required
  // for FULL):
  //      "HTTP/1.0 404 Not Found\r\n\r\n";
  //      "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n";

  if(header == NULL){
    if(strcmp(version, "HTTP/1.1") == 0){
      write(connection, "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n", 46);
    } else{
      write(connection, "HTTP/1.0 404 Not Found\r\n\r\n", 27);
    }
  } else{
    write(connection, header, strlen(header));
    write(connection, contents, strlen(contents));
  }

  // TODO: Free the headers and contents, shutdown and close both
  // sockets, return the URI of the request (possibly modified to
  // include "index.html", but not including the srv_root).

  close(socketfd);
  close(connection);
  if(header) free(header);
  if(contents) free(contents);

  // Remove srv_root/
  char *trimmed_uri = strdup(uri_loc + 8);  // Skip "srv_root/"
  free(uri_loc);  // Free the original memory
  return trimmed_uri;
  
  return uri_loc;
}
