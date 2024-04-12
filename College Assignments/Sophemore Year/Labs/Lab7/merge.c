#include <stdio.h>
#include <stdlib.h>

//Function Call
void merge(int array1[], int length1, int array2[], int length2, int result[]);
void sort(int array[]);

int main(){
	int listOneLength;
	int listTwoLength;

	//Aquire lengths
	//printf("Please insert length of first list : ");
	scanf("%d", &listOneLength);
	//printf("Please insert length of Second list : ");
	scanf("%d", &listTwoLength);
	
	int* listOne = (int*)malloc(sizeof(int)*listOneLength);
	int* listTwo = (int*)malloc(sizeof(int)*listTwoLength);
	
	//Read in list one
	//printf("Please insert %d numbers for the first list : ", listOneLength);
	for(int i = 0; i < listOneLength; i++){
		scanf("%d", &listOne[i]);
	}
	
	//Read in list Two
	//printf("Please insert %d numbers for the second list : ", listTwoLength);
	for(int i = 0; i < listTwoLength; i++){
		scanf("%d", &listTwo[i]);
	}
	
	//Print list one and list two
	printf("List 1: ");
	for(int i = 0; i < listOneLength; i++){
		printf("%d ", listOne[i]);
	}
	
	printf("\nList 2: ");
	for(int i = 0; i < listTwoLength; i++){
		printf("%d ", listTwo[i]);
	}
	
	//Create Merged List
	int* result = (int*)malloc(sizeof(int)*(listOneLength + listTwoLength));
	merge(listOne, listOneLength, listTwo, listTwoLength, result);
	
	//Print Merged List
	printf("\nMerged List: ");
	for(int i = 0; i < (listOneLength + listTwoLength); i++){
		printf("%d ", result[i]);
	}
	printf("\n");
}

void merge(int array1[], int length1, int array2[], int length2, int result[]){
	int length3 = length2 + length1;
	int listOneUse = 0;
	int listTwoUse = 0;
	
	for(int i = 0; i < length3; i++){
		if(listOneUse >= length1){
			result[i] = array2[listTwoUse];
			listTwoUse++;
		} else if(listTwoUse >= length2){
			result[i] = array1[listOneUse];
			listOneUse++;
		} else if(array1[listOneUse] < array2[listTwoUse]){
			result[i] = array1[listOneUse];
			listOneUse++;
		} else {
			result[i] = array2[listTwoUse];
			listTwoUse++;
		}
	}
}





