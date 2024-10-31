#include <stdio.h>

/*function declaration*/
int readInt();
void setBit(int number);
void unSetBit(int number);
void flipBit(int number);
int chooseBit();

int main () {
	//Aquire number and desired function call.
	int value = 0;
	printf("Please enter an integer: ");
	value = readInt();
	
	int validAnswer = -1;
	//Loop through until valid answer is given
	while (validAnswer == -1){
		printf("Do you want to set, unset, or flip a bit? (s, u, f): ");
		char answer = getchar();
		//Ignore new line
		getchar();

		switch(answer){
			case 's': {
				setBit(value);
				validAnswer = 1;
				break;
			}
			case 'u': {
				unSetBit(value);
				validAnswer = 1;
				break;
			}
			case 'f': {
				flipBit(value);
				validAnswer = 1;
				break;
			}
			default: {
				printf("Please try again.");
				break;
			}
		}
		
	}
	return 0;
}

/**
 * Function: 	readInt
 * Returns: 	an int, read from standard input. 
 **/
int readInt () {
	int c = 0;
	int i = 0;
  
	while ((c = getchar ()) != EOF && c != '\n')
	{
		if (c >= '0' && c <= '9')	
			i = i * 10 + (c - '0');	
	}

	return i;
}

//Sets the bit
void setBit(int number){
	int originalNumber = number;
	int bitPlace = chooseBit();
	//Shift bit over to correct Location
	int one = 1;
	one = one << bitPlace;
	number = number | one;
	
	printf("The result of setting bit %d in %d is %d\n", bitPlace, originalNumber, number);
}

//Unsets the bit
void unSetBit(int number){
	int originalNumber = number;
	int bitPlace = chooseBit();
	//Shift bit over to correct Location
	int one = 1;
	one = one << bitPlace;
	one = ~one;
	number = number & one;
	
	printf("The result of unsetting bit %d in %d is %d\n", bitPlace, originalNumber, number);
}

//Flip the bit
void flipBit(int number){
	int originalNumber = number;
	int bitPlace = chooseBit();
	//Shift bit over to correct Location
	int one = 1;
	one = one << bitPlace;
	number = number ^ one;
	
	printf("The result of flipping bit %d in %d is %d\n", bitPlace, originalNumber, number);
}

//Choose the bit
int chooseBit(){
	int validBit = -1;
	while(validBit == -1){
		printf("Which bit? (0-31): ");
		int bit = readInt();
		if(bit < 31 && bit > 0){
			validBit == 1;
			return bit;
		} else{
			printf("Please print a valid bit. \n");
		}
	}
	return -1;
}

