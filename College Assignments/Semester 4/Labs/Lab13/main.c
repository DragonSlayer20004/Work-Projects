#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

/* Function Declaration */
void readLine(char line[]);

int main(int argc, char **argv){
	char userName[20];
	char ip[128];
	unsigned short port = 0;
	int clientSocket = 0;

	//Gather information from user
	printf ("Enter IP: ");
	scanf ("%s", ip);
	printf ("Enter port: ");
	scanf ("%hu", &port);
	if(port < 1024 || port > 65535){
		printf ("Invalid port number: %d\n", port);
		return -1;
	}
	printf ("Enter user name: ");
	scanf ("%s", userName);
	//Read new line character
	getchar();
	
	// Create socket Address
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(address.sin_addr));
	
	// Make socket
	clientSocket = socket (AF_INET, SOCK_STREAM, 0);
	
	// Connect socket
	int fail = connect(clientSocket, (struct sockaddr *) &address, sizeof(address));
	if(fail == -1){
		printf ("Failed to connect\n");
		return -1;
	}
	
	//Start Chat
	printf ("\n*Chat Started*\n\n");
	char userMessage[1024];
	printf("%s: ", userName);
	readLine(userMessage);
	char buffer[1256];
	

	while(strcmp(userMessage, "quit") != 0){
		//Concatenate Message
		strcpy(buffer, userName);
		strcat(buffer, ": ");
		strcat(buffer, userMessage);
		
		
		write(clientSocket, buffer, strlen(buffer)+1);
		
		userMessage[1024];
		printf("%s: ", userName);
		readLine(userMessage);
		
	}
	write(clientSocket, "quit", 5);
	printf ("\n*Chat Ended*\n");
	return 0;
}

//Reads in a line of characters
void readLine(char line[]){	
	int c = 0;	
	int i = 0;

	while( (c = getchar()) != '\n' )
	{
		line[i] = c;
		i++;
	}

	line[i] = '\0';
}



