#include <stdio.h>
#include "convert.h"

int main(){
	int letter = getchar();
	
	while(letter != -1){
		if (letter != LF){
			putchar(letter);
		}
		letter = getchar();
	}
	
	return 0;
}
