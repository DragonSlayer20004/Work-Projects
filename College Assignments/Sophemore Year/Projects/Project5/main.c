/*******************************
 *
 *  Project Name: Project 5
 *  Description: This is a program that sorts through and stores large collections of movies. One can
 *  add a movie, clear all movies, delete specific movies, find all movies with the same name, find
 *  the highest revenue, change revenue, find statistics, load movie files, and save all loaded movies.
 *  Date: 4/3/2024
 *  Authors: Kyle Miller & Kyle Hinkle
 *
 *******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "movie.h"
#include "header.h"

// Main function which reads in user input and makes function calls accordingly
int main(){
	
	Movie* movieGenres[GENRES];
	// change movies from random garbage to NULL
	for(int i = 0; i < GENRES; i++){
		movieGenres[i] = NULL;
	}
	
	char userInput[100];
	
	while(strcmp(userInput, "quit") != 0){
		// Read in command
		printf("Enter command: ");
		scanf("%s", userInput);
		
		if(!strcmp(userInput, "add")){
			add(movieGenres);
		} else if (!strcmp(userInput, "clear")){
			quit(movieGenres);
		} else if (!strcmp(userInput, "delete")){
			deleteMovie(movieGenres);
		} else if (!strcmp(userInput, "find")){
			find(movieGenres);
		} else if (!strcmp(userInput, "help")){
			help();
		} else if (!strcmp(userInput, "highest")){
			highest(movieGenres);
		} else if (!strcmp(userInput, "load")){
			load(movieGenres);
		} else if (!strcmp(userInput, "revenue")){
			revenue(movieGenres);
		} else if (!strcmp(userInput, "save")){
			save(movieGenres);
		} else if (!strcmp(userInput, "statistics")){
			statistics(movieGenres);
		} else if (!strcmp(userInput, "quit")){
			// Quit ends the program.
			quit(movieGenres);
			printf("\n\nAll data cleared.\n");
			return 0;
		} else {
			printf("Unknown command: %s\n", userInput);
		}
	}
}
/**
   Insert's purpose is to add a movie into a tree in the correct location.
   If the movie already exists in the tree, then the movie is not added.
*/
Movie* insert (Movie* root, char* name, int year, int minutes, int genre, long long revenue){
	if(root != NULL){
		int difference = comparison(root, name, year);
		if(!difference){
			return root; //Duplicate movie insert.
		} else if(difference < 0){
			root->left = insert(root->left, name, year, minutes, genre, revenue);
		} else {
			root->right = insert(root->right, name, year, minutes, genre, revenue);
		}
	} else {
		// Found NULL leaf within the tree and can now replace the NULL leaf with a new branch
		root = malloc(sizeof(Movie));
		root->name = strdup(name);
		root->year = year;
		root->minutes = minutes;
		root->genre = genre;
		root->revenue = revenue;
		root->left = NULL;
		root->right = NULL;
	}
	return root;
}
/**
   Finds a movie with the given name and year found within the tree. If the 
   Movie is not present, then it returns NULL.
*/
Movie* search (Movie* root, char* name, int year){
	if (root == NULL)
		return NULL;
	int difference = comparison(root, name, year);
	// if the movies are the same, we have found the match
	if (!difference)
		return root;
	else if (difference < 0)
		return search(root->left, name, year);
	else
		return search(root->right, name, year);
}
/**
	 Deletes a Movie with the given name and year from the tree. If the Movie
     is not found within the tree, the tree is simply returned.
*/
Movie* delete (Movie* root, char* name, int year){
	// In the case that nothing is present, one will simply return NULL as nothing is there
	if (root == NULL) 
		return NULL;
	int difference = comparison(root, name, year);
	if (difference < 0)
		root->left = delete(root->left, name, year);
	else if (difference > 0)
		root->right = delete(root->right, name, year);
	else{ // if they are the same (or difference == 0)
	
		// if both children are NULL, we simply free the Movie
		if (root->right == NULL && root->left == NULL){
			free(root->name);
			free(root);
			root = NULL;
		} else if(root->right == NULL){
			// save the left child, free the root, and return the new Root
			Movie* left = root->left;
			left->name = strdup(root->left->name);
			free(root->name);
			free(root);
			root = left;
			return root;
		} else if(root->left == NULL){
			// save the right child, free the root, and return the new Root
			Movie* right = root->right;
			right->name = strdup(root->right->name);
			free(root->name);
			free(root);
			root = right;
			return root;
		} else{
			Movie* parent = root;
			Movie* child = parent->right; // the leftmost child in the RIGHT subtree
			while(child->left != NULL){
				parent = child; // accounts for the right in the beginning
				child = child->left;
			}
			// testing to see if we actually moved
			if (parent != root){
				child->left = root->left;
				parent->left = child->right;
				child->right = root->right;
				} else{
				child->left = root->left; 
				free(root->name);
				free(root);
				return child;
			}
		}	
	}
	return root;
}

/**
	Performs a preorder traversal through a movie BST
	and frees all nodes.
*/
void clear (Movie* root){
	if (root != NULL){
		clear(root->left);
		clear(root->right);
		free(root->name);
		free(root);
		root = NULL;
	}
}
/**
	Prints out the title of a movie, the year, length, genre, and revenue.
	If there is a revenue of 0, then it states the revenue is unknown.
*/
void printMovie(Movie* movie){
	if (movie->revenue > 0)
		printf("Name:\t\t%s\nYear:\t\t%d\nLength: \t%d\nGenre:   \t%s\nRevenue:\t$%lld\n\n", movie->name, movie->year, movie->minutes, GENRE_NAMES[movie->genre], movie->revenue);
	else
		printf("Name:\t\t%s\nYear:\t\t%d\nLength: \t%d\nGenre:   \t%s\nRevenue:\tUnknown\n\n", movie->name, movie->year, movie->minutes, GENRE_NAMES[movie->genre]);
}
/**
	Finds all movies with the given name, prints them out with
	printMovie, and returns the total count of matches found.
*/
int printMatches(Movie* root, char* name){
	if (root == NULL)
		return 0;
	if (!strcmp(root->name, name)){
		printMovie(root);
		return printMatches(root->left, name) + 1 + printMatches(root->right, name);
	} else
		return printMatches(root->left, name) + printMatches(root->right, name);
}

/**
	Prints all movies into the file given in the parameter beginning at the Movie*
	found within the parameter.
*/
void printAll (FILE* file, Movie* root){
	if (root == NULL)
		return;
	// throw everything into file
	fprintf(file, "%s\t%d\t%d\t%s\t%lld\n", root->name, root->year, root->minutes, GENRE_NAMES[root->genre], root->revenue);
	// recurse in a preorder traversal
	printAll(file, root->left);
	printAll(file, root->right);
}
/**
	Counts all movies present in the BST.
*/
int count (Movie* root){
	if (root == NULL)
		return 0;
	return 1 + count(root->left) + count(root->right);
}
/**
	Finds the total revenue present in the BST.
*/
long long totalRevenue (Movie* root){
	if (root == NULL)
		return 0;
	return root->revenue + totalRevenue(root->left) + totalRevenue(root->right);
}

/**
	Finds the movie with the highest revenue by searching
	through all Movies present.
*/
Movie* highestGrossing (Movie* root){
	if (root == NULL)
		return NULL;
	Movie* temp = root;
	Movie* tempLeft;
	Movie* tempRight;
	if (root->left != NULL){
		tempLeft = highestGrossing(root->left);
		if (temp->revenue < tempLeft->revenue)
			temp = tempLeft;
	}
	if (root->right != NULL){
		tempRight = highestGrossing(root->right);
		if (temp->revenue < tempRight->revenue)
			temp = tempRight;
	}
	return temp;
}

/**
	Finds the difference between a root and a given
	name and year. If negative, the given name and year
	are larger than the root. If positive, the root is larger
	than the given name and year. If 0, they are the same.
*/
int comparison(Movie* root, char* name, int year){
	int diff = strcmp(name, root->name);
	if (!diff){
		diff = year - root->year;
	}
	return diff;
}
/**
	Inserts a movie into the appropiate genre tree
	Can take in stdin for userinput
	
	ERROR need to read character by character as %s only reads up to a empty space
	tab or newline indicates end
*/
int add(Movie* movies[]){
	char name[1024];
	char buffer;
	int year = 0;
	int minutes = 0;
	char genreTemp[100]; // Temporary to read in variable 100 is genre length
	Genre genre = -1; // What we want
	long long revenue = 0;
	scanf("%c", &buffer); // clear the buffer
	printf("\nEnter name: ");
	inputToTab(name);
	printf("Enter year: ");
	scanf("%d", &year);
	printf("Enter length in minutes: ");
	scanf("%d", &minutes);
	printf("Enter genre: ");
	scanf("%s", genreTemp);
	printf("Enter revenue: ");
	scanf("%lld", &revenue);
	// write in name checking system
	if (year > 2050 || year < 1900){
		printf("Add failed: Invalid year %d\n", year);
		return 1;
		}
	if (minutes <= 0){
		printf("Add failed: Invalid length of %d\n", minutes);
		return 1;
	}
	genre = genreNumber(genreTemp);
	if(genre == -1){
		printf("Add failed: Invalid Genre %s\n", genreTemp);
		return 1;
	}
	if(revenue < 0){
		printf("Add failed: Invalid revenue %lld\n", revenue);
		return 1;
	}
	movies[genre] = insert(movies[genre], name, year, minutes, genre, revenue);
	printf("Add succeeded\n\n");
	return 0;
	}
/**
	Reads in user input until the newLine character
	is entered.
*/
char* inputToTab(char* word){
	char temp = getchar();
	int i = 0;
	for(i = 0; i < 1024 && temp != '\n'; i++){
		word[i] = temp;
		temp = getchar();
	}
	word[i] = '\0';
	return word;
}
/**
	Reads in one movie from the given file, and inserts it into the
	given Movie* movie at the correct location (based on genre).
*/
int loadMovie(FILE* file, Movie* movies[]){
	// int errors; NOT used
	char name[1024]; // 1024 is name length
	int year = 0;
	int minutes = 0;
	char genreTemp[100]; // Temporary to read in variable 100 is genre length
	Genre genre = -1; // What we want
	long long revenue = 0;
	// Name Check
	int errorCheck = readToTab(file, name);
	if(errorCheck == -1){
		return 2;
	}
	
	// Year Check
	fscanf(file, "%d", &year);
	if(year > 2050 || year < 1900){
		return 1;
	}
	
	// Clean Tab
	fgetc(file);
	// Minutes Check
	fscanf(file, "%d", &minutes);
	if(minutes <= 0)
		return 1;
	
	// Clean Tab
	fgetc(file);
	// Genre Check
	errorCheck = readToTab(file, genreTemp);
	if(errorCheck == -1)
		return 1;
	
	genre = genreNumber(genreTemp);
	if(genre == -1)
		return 1;
	
	// Revenue Check
	fscanf(file, "%lld", &revenue);
	if(revenue < 0)
		return 1;
	
	// Clean Newline
	fgetc(file);
	movies[genre] = insert(movies[genre], strdup(name), year, minutes, genre, revenue);
	return 0;
}

/**
	Reads chars from a string located within the given file
	until it surpasses 1024 chars, the newline character is reached,
	the tab character is reached, or the end of file character is reached.
	Returns the word read in.
*/
int readToTab(FILE* file, char* word){
	int temp = fgetc(file);
	int i = 0;
	for(i = 0; i < 1024 && temp != '\n' && temp != '\t'; i++){
		word[i] = temp;
		temp = fgetc(file);
		if(feof(file) != 0){
			
			return -1;
		}
	}
	word[i] = '\0';
	return 0;
}

/**
	Searches through GENRE_NAMES to see if the given genreTemp
	matches any values. If they match, the index of the genre within
	GENRE_NAMES is returned. Else, -1 is returned.
*/
int genreNumber(char* genreTemp){
	for(int i = 0; i < GENRES; i++){
		if(strcmp(genreTemp, GENRE_NAMES[i]) == 0){
			return i;
		}
	}
	return -1;
}

/**
	Takes user input of a name, genre, and year. Primary purpose
	is to find input for the delete function to use.
*/
void deleteMovie(Movie* movies[]){
	char buffer;
	char name[1024];
	char genreTemp[100];
	Genre genre = -1;
	int year;
	// error checking needed for invalid year and genre
	scanf("%c", &buffer); // clear the '\n' character
	printf("\nEnter name: ");
	inputToTab(name);
	printf("Enter a year: ");
	scanf("%d", &year);
	printf("Enter genre: ");
	scanf("%s", genreTemp);
	if (year > 2050 || year < 1900){
		printf("Delete failed: Invalid year %d\n", year);
		return;
	}
	genre = genreNumber(genreTemp);
	if (genre == -1){
		printf("Delete failed: Invalid Genre %s\n", genreTemp);
		return;
	}
	Movie* deleted = search(movies[genre], name, year);
	if (deleted == NULL)
		printf("Delete failed: %s (%d) not found in genre %s\n", name, year, genreTemp);
	else{
		deleted = delete(movies[genre], name, year);
		// reassign the genre tree at location genre
		movies[genre] = deleted;
		printf("Delete succeeded\n\n");
	}
	printf("\n");
}

/**
	Searches through all Genres and prints all movies with the same
	name as the user's given input String.
*/
void find(Movie* movies[]){
	int total = 0;
	char buffer; 
	char name[1024];
	// error checking needed for invalid name, year, and genre
	scanf("%c", &buffer); // clear the '\n' character
	printf("\nEnter name: ");
	inputToTab(name);
	for (int i = 0; i < GENRES; ++i)
		total += printMatches(movies[i], name);
	printf("%d matches found.\n\n", total);
}

/**
	Prints out all commands the user can perform and
	a brief explanation of each.
*/
void help(){
	printf("Command\n\n");
	printf("add\n");
	printf("\t\tAdds a movie to the current list\n\n");
	printf("clear\n");
	printf("\t\tClears the current list of movies\n\n");
	printf("delete\n");
	printf("\t\tsearches for a movie\n\n");
	printf("find\n");
	printf("\t\tSearches for a movie\n\n");
	printf("help\n");
	printf("\t\tPrints the list of commands\n\n");
	printf("highest\n");
	printf("\t\tPrints the highest revenue movie for each genre\n\n");
	printf("load\n");
	printf("\t\tAdds the contents of a file to the list of movies\n\n");
	printf("quit\n");
	printf("\t\tQuits the program\n\n");
	printf("revenue\n");
	printf("\t\tChanges the revenue of a move to the specified amount\n\n");
	printf("save\n");
	printf("\t\tSaves the current list of movies to a file\n\n");
	printf("statistics\n");
	printf("\t\tPrints out statistics for each genre\n\n");
}

/**
	Finds the highest grossing movie of each genre. If there exists no
	movies within the genre, or if no revenues are known in the genre, then
	it is referred to as "Unknown."
*/
void highest(Movie* movies[]){
	Movie* highest;
	printf("\n");
	for (int i = 0; i < GENRES; ++i)
	{
		highest = highestGrossing(movies[i]);
		if (highest == NULL || !highest->revenue)
			printf("%-12s\tUnknown\n", GENRE_NAMES[i]);
		else{
			printf("%-12s\t%-20s (%d) $%lld\n", GENRE_NAMES[i], highest->name, highest->year, highest->revenue);
		}
	}
	printf("\n");
}

/**
	Loads all movies from a user entered file. If the file does not exist or is
	unreadable, nothing is perfomed and the user is informed.
*/
void load(Movie* movies[]){
	int movieCount = 0;
	int errors = 0;
	char fileName[100];
	printf("\nEnter file: ");
	scanf("%s", fileName);
	
	FILE* file = fopen(fileName, "r");
	// fopen will return NULL in the case the file is inaccessable
	if (file == NULL)
		printf("File %s not found\n\n", fileName);
	else{
	int temp = 0;
	while(feof(file) == 0){
		temp = loadMovie(file, movies);
		if(temp == 0)
			movieCount++;
		else if(temp == 1)
			errors++;
	}
	printf("Loaded %d movies with %d errors\n\n", movieCount, errors);
	fclose(file);
	}
}

/*
	Frees the memory of each tree within movieGenres
*/
void quit(Movie* movieGenres[]){
	for (int i = 0; i < GENRES; ++i)
		clear(movieGenres[i]);
}

/*
	Looks for a movie with the correct name year and genre and if found successfully
	the program will change the revenue number to the new inputted amount.
*/
void revenue(Movie* movieGenres[]){
	char buffer;
	char name[1024];
	char genreTemp[100];
	Genre genre = -1;
	int year;
	long long revenue;
	scanf("%c", &buffer); // clear the '\n' character
	printf("\nEnter name: ");
	inputToTab(name);
	printf("Enter a year: ");
	scanf("%d", &year);
	printf("Enter genre: ");
	scanf("%s", genreTemp);
	printf("Enter new revenue: ");
	scanf("%lld", &revenue);
	// Checks that userInput is valid before search
	if (year > 2050 || year < 1900){
		printf("Revenue change failed: Invalid year %d\n", year);
		return;
	}
	genre = genreNumber(genreTemp);
	if (genre == -1){
		printf("Revenue change failed: Invalid Genre %s\n", genreTemp);
		return;
	}
	// Searches for the movie that is to be changed returning NULL if it doesn't exist.
	Movie* searchedMovie = search(movieGenres[genre], name, year);
	if (searchedMovie == NULL)
		printf("Revenue change failed: movie %s (%d) not found in genre %s\n\n", name, year, genreTemp);
	// Changes the revenue of the found movie
	else{
		searchedMovie->revenue = revenue;
		printf("Revenue change successful\n\n");
	}
}

/*
	Takes in a name for the new file prints out all the trees in preorder sort.
*/
void save(Movie* movieGenres[]){
	char newFileName[100];
	printf("\nEnter a file: ");
	scanf("%s", newFileName);
	FILE* newFile = fopen(newFileName, "w");
	// If it fails to open the new file it throws an error.
	if(newFile == NULL){
		printf("File %s not writable", newFileName);
	}
	// Goes through every individual tree within movieGenres and prints it.
	for (int i = 0; i < GENRES; ++i){
		printAll(newFile, movieGenres[i]);
	}
	printf("Save succeeded\n\n");
	fclose(newFile);
}

/*
	Prints to screen how many movies are within each genre as well as the total revenue generated
	by each genre.
*/
void statistics(Movie* movieGenres[]){
	// Loops over every individual tree.
	for (int i = 0; i < GENRES; ++i){
		int total = count(movieGenres[i]);
		long long revenue = totalRevenue(movieGenres[i]);
		printf("%-12s\t", GENRE_NAMES[i]);
		printf("Count: %10d\t",total);
		printf("Total revenue: $%lld\n", revenue);
	}
	printf("\n");
}
