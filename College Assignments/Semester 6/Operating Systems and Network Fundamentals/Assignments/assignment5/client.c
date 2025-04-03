#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"

#define BUFFER_MAX_SIZE 4096

/* Given a struct with server information, return a dynamically allocated
   string of the IP address in dotted decimal or colon hexadecimal notation.
   Consult Chapter 4, as well as /usr/include/netdb.h and
   /usr/include/arpa/inet.h as needed. Use inet_ntop() to get the formatted
   string based on the address's ai_addr field.
 */
char *
addr_string (struct addrinfo *server)
{
  // Return safely in case server is NULL:
  if (server == NULL)
    return strdup ("no address information");
  
  /*
    server->ai_addrlen will technically work but it is bad practice.
    server->ai_addrlen returns the full size including port and other information.address
    we want just the size of afnet 4 or 6 so we check the family for the correct type size.address

    If true then set length for Inet4 otherwise set it to be for 6. (SHOULD NEVER BE ANYTHING BUT THOSE 2)
  */ 
  int addr_len = (server->ai_family == AF_INET) ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;


  // Create buffered address based on address length.
  char *address = malloc(sizeof(char) * addr_len);
  if (address == NULL) {
      perror("malloc failed");
      return strdup("Memory allocation error.");
  }

  // Pointer for where the ip address is stored.
  void *addressPointer = NULL;

  /*
    ai_addr holds more than just the ip address and reads in the wrong information.addr_len
    We need to grab just the information we want which is either the sin_addr or sin6_addr.
    We will grab the data within the address but it is not sa.data for we need to know what type it is before we can get the ip address.
    Struct data is different if it is INET4 (4 bytes) or INET6 (16 bytes).
    We need to specify which address struct we are reading from and change the parameters name to match that struct.

    As mentioned above it can only be INET4 or INET6 anything else is invalid and we free address on fail.
  */
  if(server->ai_family == AF_INET){ //Grab the pointer for INET4
    addressPointer = &((struct sockaddr_in *)server->ai_addr)->sin_addr;
  } else if (server->ai_family == AF_INET6){ //Grab the pointer for INET6
    addressPointer = &((struct sockaddr_in6 *)server->ai_addr)->sin6_addr;
  } else {
    free (address);
    return strdup("Invalid address family.");
  }

  /*
    af: address family which is just the ai_family in the server. Can be INET4 or 6
    src: address that we are reading from.
    dst: pointer to our bufferered address and where it is being placed.
    size: size of the buffer which is just server address length.

    This is assuming that server info is automatically adjusted to use either afnet4 or 6 but not both.

    If it returns NULL then inte_ntop failed and we need to free address.
  */
  if(inet_ntop(server->ai_family, addressPointer, address, addr_len) == NULL){
    free(address);
    return strdup ("inet_ntop failed to generate string");
  }

  return address;
}

/* Given the server address info, return a dynamically allocated string with
   its transport-layer protocol, IP version, and IP address. For instance,
   querying otterbein.edu over TCP/IPv4 should return:
     "TCP IPv4 205.133.226.138"
   Use addr_string() to get the formatted address string, concatenate it to
   the string to return, then free the result from addr_string(). If the
   passed server parameter is NULL, use malloc() and strcpy() to return a
   dynamically allocated copy of "no address information".
 */
char *
serv_string (struct addrinfo *server)
{
  // Return safely in case server is NULL:
  if (server == NULL)
    return strdup ("no address information");

  // Get ai_protocol based on protocol.
  // char *protocol = (server->ai_protocol == IPPROTO_TCP) ? "TCP" : "UDP";

  // Get ai_protocol based on ai_socktype.
  char *protocol = (server->ai_socktype == SOCK_STREAM) ? "TCP" : "UDP";

  // Get ai_family
  char *family = (server->ai_family == AF_INET) ? "IPv4" : "IPv6";

  // Get the formatted address string.
  char *ipAddress = addr_string(server);

  // Allocate 10 bytes for protocol (TCP or UDP), IP version (IPv4 or
  // IPv6), along with spaces and the ending null byte. In addition,
  // include enough space for an IP address (either v4 or v6).
  // Protocol family ipAddress \0 2 spaces and null character thus plus 3.
  int totalSize = (strlen(protocol) + strlen(family) + strlen(ipAddress) + 3);
  char *address = malloc (sizeof(char) * totalSize);
  if (address == NULL) {
    free(ipAddress);
    return strdup("Memory allocation failed");
  }

  // Now, use strncat() to build the string based on information from the server struct.
  /*
    Use snprintf to combine address information together. (Easier than spamming strcat for each one.)
    total size includes spaces and null character.
    Combine the string versions of protocol, family and ipAddress.
  */
  snprintf(address, totalSize, "%s %s %s", protocol, family, ipAddress);

  //Free ipAddress
  free(ipAddress);

  return address;
}

/* Given a hostname string, use getaddrinfo() to query DNS for the specified
   protocol parameters. Boolean values indicate whether or not to use IPv6
   (as opposed to IPv4) or TCP (as opposed to UDP). For this assignment, the
   protocol will only be "http", though it could be others in a more complete
   implementation. Return the pointer to the linked list of server results.
 */
struct addrinfo *
get_server_list (const char *hostname, const char *proto, bool tcp, bool ipv6)
{
  struct addrinfo *server_list = NULL;
  struct addrinfo hints;

  // Initialize all the fields inside hints to 0.
  memset (&hints, 0, sizeof (hints));

  // Set family based on the ipv6 flag.
  hints.ai_family = (ipv6) ? AF_INET6 : AF_INET;

  // Set socket type based on the tcp flag.
  hints.ai_socktype = (tcp) ? SOCK_STREAM : SOCK_DGRAM;

  // Set the protocol based on tcp flag.
  hints.ai_protocol = (tcp) ? IPPROTO_TCP : IPPROTO_UDP;

  // Query DNS for the specified hostname and protocol.
  int result = getaddrinfo(hostname, proto, &hints, &server_list);

  // Check if getaddrinfo was successful. If 0 it succeeded otherwise use the function gai_strerror to get an error. (gai = getaddrinfo)
  if (result != 0) { //
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
      return NULL;  // Return NULL if there's an error
  }
  return server_list;
}

/* Use an HTTP/1.0 request to retrieve the requested file with a GET request.
   Return the first line of the header and the content type. To get the type,
   use strtok() to split the lines of the header at "\n". Then look for the
   line that beging "Content-Type: ". Set the call-by-reference paramter type
   to be the remainder of that line. Be sure to trim off the "\r" at the end.
 */
char *
web (const char *hostname, char *proto, const char *file, char **type,
     bool ipv6)
{
  // To start, call get_server_list(), then loop through the results trying
  // to create a socket connection. When a connection is successfully
  // established, send "GET file HTTP/1.0\r\n\r\n" for the file specified.
  // Write the request to the socket and read the result into the buffer.
  char buffer[BUFFER_MAX_SIZE];  // Should not need more that this
  memset (buffer, 0, sizeof (buffer));

  // Check if TCP or UDP
  bool tcp = true;

  // Get addressInfo.
  struct addrinfo *addressInfo = get_server_list(hostname, proto, tcp, ipv6);
  if (addressInfo == NULL) {
      return strdup("Failed to get addressInfo");
  }

  // Try to connect to one of the provided addresses
  int fd = -1;
  // temp holder since we keep changing the next address we are reading.
  const struct addrinfo *addr;
  /*
    For loop starts as addressInfo then looks through all the other addresses in it until it reaches NULL.
    getaddrinfo automatically populates the additional address so we just need to look through all of them until we find one that successfully connects.
  */ 
  for (addr = addressInfo; addr != NULL; addr = addr->ai_next) {
    // Try to create a valid file descriptor for socketing.
    fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (fd == -1) {
        continue; // Try the next address if socket creation fails
    }
    if (connect(fd, addr->ai_addr, addr->ai_addrlen) == 0) {
        break; // Successfully connected
    }
    close(fd);
    fd = -1;
  }

  if (fd == -1) {
    return strdup("Failed to connect to host");
  }

  // Create the request.
  char request[512];

  snprintf(request, sizeof(request), "GET %s HTTP/1.0\r\n\r\n", file);
  //Adding hostname might help but it leads to more errors when tested.
  //snprintf(request, sizeof(request), "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", file, hostname);


  // Send the get request.
  if (write(fd, request, strlen(request)) == -1) {
    close(fd);
    return strdup("Failed to send request");
  }

  // Read the HTTP response
  ssize_t bytes_read = read(fd, buffer, BUFFER_MAX_SIZE - 1);
  close(fd);
  if (bytes_read <= 0) {
      return strdup("Failed to read response");
  }
  buffer[bytes_read] = '\0';

  /*
    Extract the status line (first line of response)
    Grab the first line of the header via strtok.
  */
  char *status_line = strtok(buffer, "\r\n");
  if (status_line == NULL) {
      return strdup("Invalid response");
  }

  // Copy the status line to return
  char *response = strdup(status_line);

  // Look for the Content-Type header
  char *line;
  /*
    First time using strtok but it should work.
    first input is the string we are manipulating and the second input are the delimiters.
    Any time it finds a delimiter it replaces it with a '\0' and returns the token created.
    Thus the line "hello\n\r\n goodbye" returns "hello\n" The can also be multiple delimiter characters in the same parameter.
    In the case that there is no delimiter found within the string it will instead return the whole string as the token.

    If using NULL as the first parameter it continues off from where it started at it.
    In this case we used strtok on buffer and grabbed the first line. Now we are looping through the rest of the buffer
    using the internal pointer within strtok to track our position.
    By using the whileloop we can search through every line within the buffer until there is nothing left to read with strtok returning NULL.
  */ 
  while ((line = strtok(NULL, "\r\n")) != NULL) {
      if (strncasecmp(line, "Content-Type: ", 14) == 0) {
          *type = strdup(line + 14); //Plus 14 to exlude "Content-Type: "

          // Remove trailing "\r" if present strchr finds the extra char.
          char *carriage = strchr(*type, '\r');
          if (carriage) {
              *carriage = '\0';
          }
          break; // Found Content-Type, exit loop
      }
  }

  //Free addressInfo.
  freeaddrinfo(addressInfo);

  return response;
}
