#include <stdio.h>

//Function Call
char toUpper(char c);

/**
	Main method
**/
int main(){
	//Get Word
	char word[1024];
	int endLocation = 1023;
	
	printf("Enter a string: ");
	int value = getchar();
	
	for(int i = 0; i < 1023 && value != '\n' && value != EOF; i++){
		word[i] = value;
		endLocation = i;
		value = getchar();
	}
	
	//Set last variable to end;
	endLocation++;
	word[endLocation] = '\0';

	//Compares start to end. End is - 1 to ignore end of string symbol.
	for(int i = 0; i < endLocation / 2; i++){
		if(toUpper(word[i]) != toUpper(word[endLocation - i - 1])){
			printf("\nThe string \"%s\" is not a palindrome.\n", word);
			return 0;
		}
	}
	printf("The string \"%s\" is a palindrome.\n", word);
	return 0;
}

// Capitalize letter if needed
char toUpper(char c){
	if(c >= 'a' && c <= 'z'){
		c = c - 'a' + 'A';
	}
	return c;
}
