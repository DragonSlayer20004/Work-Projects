#include <stdio.h>
#include "lab9.h"
#include <string.h>

int main()
{
	char input;
	char name[NAME_LENGTH];
	char email[EMAIL_LENGTH];
	int age;
	ContactList list;
	Contact* contactPointer = NULL;
	
	list.capacity = 5;
	list.contacts = (Contact*)malloc(list.capacity*sizeof(Contact));
	list.size = 0;
		
	while(  (input = menu()) != 'Q' )
	{
		switch( input )			
		{
			case 'A':	// Add contact
				printf("\tEnter name: ");
				getLine(name);
				printf("\tEnter e-mail: ");
				getLine(email);
				printf("\tEnter age: ");
				scanf("%d", &age);		
				addContact(name, email, age, &list);
				break;
			case 'F':	// Find contact
				printf("\tEnter name: ");
				getLine(name);
				contactPointer = findContact(name, &list);
				if( contactPointer == NULL )
					printf("\t\"%s\" not found\n", name);
				else
					printContact( contactPointer );
				break;			
			case 'P': 	// Print all contacts
				printAllContacts(&list);
				break;
		}
	}

	return 0;
}

char menu()
{  
	int choice;

	// Print menu  
	printf("A - Add a contact\n");
	printf("F - Find and display a contact\n");
	printf("P - Print contact list\n");
	printf("Q - Quit\n");
	printf("Your choice: ");

	// Get input
	while (isspace(choice = getchar()));
	while (getchar() != '\n');

	return toupper(choice);
}

void printAllContacts(const ContactList* list)
{
	int i = 0;

	if (list->size > 0)
	{
		// Print everything
		for (i = 0; i < list->size; ++i)
		{
		  printContact(&list->contacts[i]);
		  printf("\t----------------\n");
		}
	}
	else
		printf("\tContact list is empty!\n");
}

void getLine(char buffer[])
{
	int i = 0;
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		buffer[i++] = c;
	buffer[i] = '\0';
}

/************* Functions You Have to Write *************/

/* Adds a contact
 * Resizes the array to fit the new contact, if necessary
 * If a new contact requires the array to be reallocated, it should be doubled in size
 */
void addContact(char* name, char* email, int age, ContactList* list){
	if(list->size >= list->capacity){
		//increase capacity
		list->capacity = list->capacity * 2;
		list->contacts = realloc(list->contacts, (list->capacity*sizeof(Contact)));
	}
	
	list->contacts[list->size].name = strdup(name);
	list->contacts[list->size].email = strdup(email);
	list->contacts[list->size].age = age;
	list->size++;
}

/* Searches the list for the contact name's passed in.
 * Returns the position of the contact by that name.
 */
Contact* findContact(char* name, const ContactList* list){
	int found = -1;
	for(int i = 0; i < list->size && found == -1; i++){
		if(strcmp(list->contacts[i].name, name) == 0){
			found = i;
		}
	}
	
	if(found == -1){
		return NULL;
	}
	return &list->contacts[found];
}

/* Print the contact given as a parameter 
 * The format should be:
 * Name: (name) <(email)> is (age) years old.
*/
void printContact(const Contact* toPrint){
	printf("\t%s <%s> is %d years old.\n", toPrint->name, toPrint->email, toPrint->age);
}










