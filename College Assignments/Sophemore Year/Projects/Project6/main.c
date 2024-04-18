/*******************************
 *
 *  Project Name: Project Six
 *  Description: Webserver
 *  Date: 04/19/2024
 *  Authors: Kyle Hinkle, Zachary Stray
 *
 *******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
    chmod +x filename.exe to make executable file work
	To test the program run the server and go to the link below (might have to change port number).
	http://127.0.0.1:23034/ will send a call to the server and then open the site.
	nc 127.0.0.1 23034 then GET / HTTP/1.0 command to view sent information.
	nc 127.0.0.1 23034 HEAD / HTTP/1.0 to view header information.
*/

/* Function Declaration */
void readLine(char line[], char buffer[]);
void getFile(char line[], char buffer[]);
void headFile(char line[], char buffer[]);
char* getContentType(char path[]);
//char * getExtention(char * path);

int main(int argc, char **argv)
{
	//Initial Error Checking
	//More input than expected.
	if(argc != 3){
		printf("Usage: ./server <port> <path to root>\n");
		return -1;
	}
	//Grab port number and check if valid.
	unsigned short port = atoi(argv[1]);
	if(port < 1024 || port > 65535){
		printf("Bad port: %hu\n", port);
		return -1;
	}
	//Use chdir() and if it fails return failure message
	int fail = chdir(argv[2]);
	if(fail != 0){
		printf("Could not change to directory: %s\n", argv[2]);
		return -1;
	}
	
	//Variables for socketing and Address.
	int serverSocket = 0;
	int clientSocket = 0;
	char buffer[1024];
	int length = 0;
	int value = 1;
	struct sockaddr_in address;
	struct sockaddr_storage otherAddress;
	socklen_t otherSize = sizeof (otherAddress);

	// Make socket
	serverSocket = socket (AF_INET, SOCK_STREAM, 0);
	
	// Make it able to reuse ports	
	setsockopt (serverSocket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));

	// Bind socket to port and local IP
	memset (&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	bind (serverSocket, (struct sockaddr*)&address, sizeof(address));

	// Listen
	listen (serverSocket, 1);
	
	/**
		Loops infinitely waiting for connections and sending the specified file information
		Before looping back into accepting state.
	*/
	while(1){
		// Accept	
		clientSocket = accept (serverSocket, (struct sockaddr *) &otherAddress, &otherSize);	
		
		printf ("Got a client: \n");
		
		// Receive messages
		length = read( clientSocket, buffer, sizeof(buffer) - 1);
			
			buffer[length] = '\0';
			char command[256];
			char path[1024];
			char version[256];
			
			//Make empty
			version[0] = '\0';
			
			sscanf(buffer, "%s%s%s", command, path, version);
			//Print message for recieved command
			printf("\t %s %s %s\n", command, path, version);
			
			//Waits for valid request i.e. HEAD or GET
			char* newPath = path;
			if(strcmp(command, "GET") == 0 || strcmp(command, "HEAD") == 0){
			    if(path[strlen(path)-1] == '/'){
			        strcat(path, "index.html");
			    }
			    if(path[0] == '/'){
			        newPath++;
			    }
			    
			    
			    
			    //If it fails to pen a valid file it will throw out an error message.
			    int file = open(newPath, O_RDONLY);
			    if (file == -1){
			        printf("404 file not found\n");
			        char* errorMessage = "HTTP/1.1 404 Not Found\r\n\r\n";
			        write(clientSocket, errorMessage, strlen(errorMessage));
			        //If GET request with throw out error page
			        if(strcmp(command, "GET") == 0){
			            char htmlError[512];
			            sprintf(htmlError, "<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD><BODY><H1>Not Found</H1><P>The requested URL %s was not found on this server.</P></BODY></HTML>", newPath);   
			        write(clientSocket, htmlError, strlen(htmlError));
			        }
			    } //File successfully opened 
			    else{
			    	//Send file information
			        //Aquire file Size
			        int fileSize;
			        struct stat st;
                    stat(newPath, &st);
                    fileSize = st.st_size;
			        
			        //Aquire Content-Type
			        char* contentType = getContentType(newPath);
			        
			        //Write to client
			        char fileHeader[1024];
			        
			        sprintf(fileHeader, "HTTP/1.0 200 OK\r\nContent-Length: %d\r\nContent-Type: %s\r\n\r\n", fileSize, contentType);
			        write(clientSocket, fileHeader, strlen(fileHeader));
			        //All Information for head complete.
			        if(strcmp(command, "HEAD") == 0){
			        	printf("Sent head: %s\n", newPath);
			        } else{
						unsigned char buffer[512];
						
						//run until there are no more bytes to read.
						int fail;
						do{
							fail = read( file, buffer, sizeof(char)*512 );
							if(fail < 512){
								buffer[fail] = '\0';
							}
							//Writes up to 512 characters at a time.
							write(clientSocket, buffer, fail);
						}
						while(fail == 512);
			        	printf("Sent file: %s\n\n", newPath);
			        }
			    }
			    close(file);
			    
			} else {
			    printf("Unknown Command:");
			}
			//Once it runs HEAD or GET go back to accepting state.
			// Clean up
			close (clientSocket);
		}

	printf ("\n*Connection Closed*\n");
	// Clean up
	close (clientSocket );
	
	// Clean up
	close (serverSocket );
	
	return 0;
}

/**
	Gets just the file information not content
*/
void headFile(char line[], char buffer[]){
    printf("Made it to HEAD!\n");
    if(1 == 1){
	    printf("fail\n");
	}
}

/**
    Reads in a line from buffer.
    Probably going to be deleted.
*/
void readLine(char line[], char buffer[])
{	
	int c = 0;	
	int i = 0;

	while( (c = buffer[i]) != '\n' )
	{
		line[i] = c;
		i++;
	}

	line[i] = '\0';
}

/**
    Reads up to the end and finds out what the extension is.
*/
char* getContentType(char path[]){
    char extension[16];
    int extensionStart = -1;
    //Loops through to find where extension starts.
    for(int i = 0; i < 16 && extensionStart == -1; i++){
        if(path[i] == '.'){
            i++;
            extensionStart = i;
            for(int j = 0; i < 16 && path[i] != EOF; i++, j++){
                extension[j] = path[i];
            }
        }
    }
    
    //If there is no extension found.
    if(extensionStart == -1){
        return "No Content-Type";    
    }
    //Found extension now identify extension.
    if(strcmp(extension, "html") == 0 || strcmp(extension, "htm") == 0){
        return "text/html";
    } else if(strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0){
        return "image/jpeg";
    } else if(strcmp(extension, "txt") == 0 || strcmp(extension, "c") == 0 || strcmp(extension, "h") == 0){
        return "text/plain";
    } else if(strcmp(extension, "gif") == 0){
        return "image/gif";
    } else if(strcmp(extension, "png") == 0){
        return "image/png";
    } else if(strcmp(extension, "pdf") == 0){
        return "application/pdf";
    } else {
        //No extensions found of known types.
        return "No Content-Type";
    }
}




