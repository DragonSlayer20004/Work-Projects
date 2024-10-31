#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* Function Declaration */


int main(int argc, char **argv){
	//Error check if there are more or less than 2 command line arguments
	if(argc != 2){
		printf("Usage: %s <filename>\n", argv[0]);
		return -1;
	}
	char* fileName = argv[1];
	
	//Error check if file exists.
	int file = open(fileName, O_RDONLY);
	if(file < 0){
		printf("%s: %s: No such file or directory\n", argv[0], fileName);
		return -1;
	}
	
	unsigned int sum = 0;
	unsigned int byteChunks = 0;
	unsigned char buffer[512];
	
	//run test until there are no more bytes to read.
	int fail;
	do{
		fail = read( file, buffer, sizeof(char)*512 );
		for(int i = 0; i < fail; i++){
			sum += buffer[i];
		}
		byteChunks++;
	}
	while(fail == 512);
		
	//Variable to hold 2^16
	int twoExpo = (1 << 16);
	int r = (sum % twoExpo) + (sum / twoExpo);
	int s = (r % twoExpo) + (r / twoExpo);
	
	printf("%d %d %s\n", s, byteChunks, fileName);
	close(file);
	return 0;
}



