#include <stdio.h>
#include <stdlib.h>

/**
	Struct Declaration
*/
typedef struct LinkNode{
	int value;
	struct LinkNode* next;
} LinkNode;

/**
	Function Declaration
*/
LinkNode* add(LinkNode* head, int value);
LinkNode* delete(LinkNode* head, int value);
void print(LinkNode* head);
void empty(LinkNode* head);

/**
	Main Method
*/
int main(){
	LinkNode* head = NULL;
	int userInput = 0;
	
	while(userInput != 4){
		printf("1. Add new element\n2. Delete element\n3. Print list\n4. Exit\n");
		scanf("%d", &userInput);
		getchar();
		int number;
		switch(userInput){
			case 1:
				scanf("%d", &number);
				getchar();
				head = add(head, number);
				break;
			case 2:
				scanf("%d", &number);
				getchar();
				head = delete(head, number);
				break;
			case 3:
				print(head);
				break;
			default:
				//Do nothing
				break;
		}
	}
	return 0;
}

// Adds value to the head of the list
// Returns new head
LinkNode* add(LinkNode* head, int value){
	if(head == NULL){
		head = malloc(sizeof(LinkNode));
		head->value = value;
		head->next = NULL;
	} else {
		LinkNode* newHead = malloc(sizeof(LinkNode));
		newHead->value = value;
		newHead->next = head;
		head = newHead;
	}
	return head;
}

// Delete first occurrence of value from the list
// Returns new head
LinkNode* delete(LinkNode* head, int value){
	if(head != NULL){
		if(head->value == value){
			LinkNode* next = head->next;
			free(head);
			head = next;
		} else{
			head->next = delete(head->next, value);
		}
	}
	return head;
}

// Print all the values in the list
void print(LinkNode* head){
	while(head != NULL){
		printf("%d ", head->value);
		head = head->next;
	}
	printf("\n");
}

// Free all the memory in the list
void empty(LinkNode* head){
	if(head->next == NULL){
		free(head);
	} else {
		empty(head->next);
		free(head);
	}
}






