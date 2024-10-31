/*******************************
 *
 *  Project Name: Project 3
 *  Description: Write a subset of the standard C string library
 *  Date: Friday, February 23, 2024 at 11:59 p.m. 
 *  Authors: Kyle Hinkle, An Tran
 *
 *******************************/


#include <stdio.h>

/* 
   Description:	 Copies the characters from source into destination. 
				
   Parameters:	source     	- string to be added
				destination - destination string. Destination must be same size or greater than source
				
   Returns:	new string
*/ 

char* new_strcpy(char* destination, const char* source){
	int i;
	for(i = 0; source[i] != '\0'; i++){
		destination[i] = source[i];
	}
	//Close String
	destination[i] = '\0';
	
	return destination;
}

/* 
   Description:	 Copies the first n characters from source into destination. If the function hits a null character in source before it has copied n characters, the remaining characters are filled with null characters.  
				
   Parameters:	source     	- string to be added
				destination - destination string. Destination must be same size or greater than source
				n 			- number of characters to be added
   Returns:	new string
*/ 
char* new_strncpy(char* destination, const char* source, size_t n){
	int i;
	int foundEnd = 0;
	for(i = 0; i < n ; i++){
		if(foundEnd){
			destination[i] = '\0';
		} else {
			destination[i] = source[i];
			if(source[i] == '\0'){
				foundEnd = 1;
			}
		}
	}
	return destination;
}

/* 
   Description:	 Compares two strings 
				
   Parameters:	string1  
				string2		

   Returns:	A positive integer if string1 comes after string2 alphabetically, negative if string1 comes before string2 alphabetically, and 0 if the two strings are equal.
*/ 
int new_strcmp(const char* string1, const char* string2){
	for(int i = 0; string1[i] != '\0' && string2[i] != '\0'; i++){
		if(string1[i] - string2[i] != 0){
			return string1[i] - string2[i];
		}
	}
	return 0;
}

/* 
   Description:	 Compares two strings, except comparing at most the first n characters.  
				
   Parameters:	string1  
				string2	
				n - size of substring to be compared	

   Returns:	A positive integer if string1 comes after string2 alphabetically, negative if string1 comes before string2 alphabetically, and 0 if the two strings are equal.
*/ 
int new_strncmp(const char* string1, const char* string2, size_t n){
	for(int i = 0; string1[i] != '\0' && string2[i] != '\0' && i < n; i++){
		if(string1[i] - string2[i] != 0){
			return string1[i] - string2[i];
		}
	}
	return 0;
}

/* 
   Description:	Adds the string contained in source to the end of the string contained in destination.   
				
   Parameters:	destination - destination string 
				source - source string	

   Returns:	A pointer to destination.
*/ 
char* new_strcat(char* destination, const char* source){
	int length = 0;
	for (int i = 0; destination[i] != '\0'; ++i) {
		++length;
	}
	
	int i = 0;
	for (; source[i] != '\0'; ++i) {
		destination[i+length] = source[i];
	}
	destination[length+i] = '\0';
	
	return destination;
}

/* 
   Description:	Adds the string contained in source to the end of the string contained in destination, but adding a maximum of n characters. 
				
   Parameters:	destination - destination string 
				source - source string
				n - maximum characters to be added	

   Returns:	A pointer to destination.
*/ 
char* new_strncat(char* destination, const char* source, size_t n){
	int length = 0;
	for (int i = 0; destination[i] != '\0'; ++i) {
		++length;
	}
	
	int i = 0;
	for (; source[i] != '\0' & i < n; ++i) {
		destination[i+length] = source[i];
	}
	
	destination[length+i] = '\0';
	
	return destination;

}

/* 
   Description:	Find length of a given string
   Parameters:	string
   Returns:	The number of characters in string before the null character. 
*/ 
size_t new_strlen(const char* string){
	int i;
	for(i = 0; string[i] != '\0'; i++);
	return i;
}

/* 
   Description:	Find a character a given string
   Parameters:	string
   				character - character to look for
   Returns:	A pointer to the first occurrence of character in string or a NULL pointer if character cannot be found. 
*/ 
char* new_strchr(const char* string, int character){
	int i;
	for(i = 0; string[i] != '\0'; i++){
		if(string[i] == character){
			return (char*)&string[i];
		}
	}
	//In case character is 'end of string'
	if(string[i] == character){
		return (char*)&string[i];
	}
	return NULL;
}

/* 
   Description:	Find a string within a given string
   Parameters:	haystack - given string
   				needle - string to look for
   Returns:	A pointer to the first occurrence of the string contained in needle in haystack or a NULL pointer if needle cannot be found.
*/ 
char* new_strstr(const char* haystack, const char* needle){
	int match = 1;
	
	for (int i = 0; haystack[i] != '\0'; ++i) {
		match = 1;
		for (int j = 0; needle[j] != '\0' && haystack[i+j] != '\0' && match; ++j) {
			
			
			if (haystack[i+j] != needle[j]) {
				match = 0;
			}
		}
		
		if (match) {
			return (char*)&haystack[i];
		}
	}
	
	return NULL;
}
