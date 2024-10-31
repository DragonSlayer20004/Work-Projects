#include <stdio.h>

//Function Call
void pascal(int);
long int factorial(int);
int readInt();

/**
	Main method
**/
int main(){
	//Recieve user input
	printf("How many rows are to be made : ");
	int rows = readInt();
	
	//Checks for valid input
	if(rows < 0 || rows > 20){
		printf("Invalid input\n");
		return -1;
	}
	
	//Loops through all the rows
	for(int i = 0; i < rows; i++){
		pascal(i);
		printf("\n");
	}
	
	return 0;
}

/**
	Calculates where the current number is based on the current row and the current column is and prints it.
**/
void pascal(int row){
	for(int column = 0; column <= row; column++){
		long int number = (factorial(row)/(factorial(row - column)*factorial(column)));
		printf("%ld ", number);
	}
}

/**
	Keeps multiplying until it gets to 1 or lower.
**/
long int factorial(int number){
	if(number <= 1){
		return 1;
	} else{
		return number*factorial(number-1);
	}
}

/**
 * Function: 	readInt
 * Returns: 	an int, read from standard input. 
 **/
int readInt()
{
	int c = 0;
	int i = 0;
	int sign = 1;
  
	while ((c = getchar ()) != EOF && c != '\n' && c != ' ')
	{
		if (c == '-')
			sign = -1;
		else if( c >= '0' && c <= '9')	
			i = i * 10 + (c - '0');	
	}

	return sign*i;
}
