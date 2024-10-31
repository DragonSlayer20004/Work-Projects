#include <stdio.h>

/* Kyle Hinkle*/
int main() {
	for (int i = 1; i <= 10; i++){
		for (int j = 0; j < i; j++){
			printf("*");
		}
		printf("\n");
	}
}
