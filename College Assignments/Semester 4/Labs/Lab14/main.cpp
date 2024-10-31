#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

/* Function Declaration */
string getName(int value);
string getTens(int value);
string getTeens(int value);


int main(int argc, char **argv){
	//Numbers are between 0-999 inclusive.
	cout << "Enter a number: (0-999) ";
	int value = 0;
	cin >> value;
	
	int onePlace = value % 10;
	int secondPlace = (value % 100) / 10;
	int thirdPlace = value /100;
	
	cout << onePlace << " " << secondPlace << " " << thirdPlace << endl;
	
	//Generate String Number
	string answer = "";

	if(value < 10){
		answer = getName(value);
	} else {
		if (thirdPlace != 0) {
			answer = getName(thirdPlace) + " hundred ";
		}
		if (secondPlace > 1) {
			answer = answer + getTens(secondPlace) + " " + getName(onePlace);
		} else if(secondPlace == 1){
			answer = answer + getTeens(onePlace);
		} else if(onePlace > 0){
			answer = answer + getName(onePlace);
		}
	} 
	
	cout << "Your number in words: " << answer << endl;
	return 0;
}

/**
	Return base number name.
*/
string getName(int value)
{
	switch( value )
	{
	case 0: return "zero";
	case 1: return "one";
	case 2: return "two";
	case 3: return "three";
	case 4: return "four";
	case 5: return "five";
	case 6: return "six";
	case 7: return "seven";
	case 8: return "eight";
	case 9: return "nine";
	}
	
	return "";
}

/**
	Returns numbers in the tenth place
*/
string getTens(int value){
	switch( value )
	{
	case 1: return "ten";
	case 2: return "twenty";
	case 3: return "thirty";
	case 4: return "fourty";
	case 5: return "fifty";
	case 6: return "sixty";
	case 7: return "seventy";
	case 8: return "eighty";
	case 9: return "ninety";
	}
	
	return "";
}

/**
	Return numbers in the teens place
*/
string getTeens(int value){
	switch( value )
	{
	case 0: return "ten";
	case 1: return "eleven";
	case 2: return "twelve";
	case 3: return "thirteen";
	case 4: return "fourteen";
	case 5: return "fifteen";
	case 6: return "sixteen";
	case 7: return "seventeen";
	case 8: return "eighteen";
	case 9: return "nineteen";
	}
	
	return "";
}






