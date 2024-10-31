#include <stdio.h>
#include <string.h>

/*function declaration*/
void backwards(char **argv, int start, int stop);
void reverse(char **argv, int start, int stop);

// argc: number of command line arguments
// argv: array of strings (contains the command line arguments)
int main(int argc, char **argv){
	printf("\n");
	for(int i = 1; argv[1][i] != '\0'; i++){
		if(argv[1][i] == 'b'){
			backwards(argv, 2, argc);
		}
		else if(argv[1][i] == 'r'){
			reverse(argv, 2, argc);
		}
	}
	//Print New Sentence
	for(int j = 2; j < argc; j++){
		printf("%s ", argv[j]);
	}
	printf("\n\n");
}

// Makes each word from start up to but not including stop backwards. 
// Note that you will always start with index 2, since index 0 is the
// name of the executable and index 1 is the list of flags. 
// The stopping point is argc.
void backwards(char **argv, int start, int stop){
	for(int i = start; i < stop; i++){
		int wordLength = strlen(argv[i]);
		for(int j = 0; j < wordLength / 2; j++){
			char temp = argv[i][j];
			argv[i][j] = argv[i][wordLength - j - 1];
			argv[i][wordLength - j -1] = temp;
		}
		argv[i][wordLength] = '\0';
	}
}

// Reverses the order of the words in the sentence from start up to
// but not including stop. Again, the list of words starts at index 2
// and ends at argc.
void reverse(char **argv, int start, int stop){
	for(int i = start; i < (stop+start) / 2; i++){
		char* temp = argv[i];
		argv[i] = argv[stop - i + start - 1];
		argv[stop - i + start - 1] = temp;
	}
}

