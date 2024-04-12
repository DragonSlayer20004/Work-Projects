#include<stdio.h>

/*function declaration*/
int fileToArray(unsigned char message[]);
void tiling(unsigned char key[], int arraySize, int originalSize);
unsigned char rotate(unsigned char letter, int count);
int bits(unsigned char letter);
int setBit(int number);

/*constants*/
#define SIZE 2048

/*	Kyle Hinkle
	Main method the runs everything.
*/
int main(){
	unsigned char message[SIZE];
	unsigned char key[SIZE];
	
	//Read in message and key and find their size
	int messageSize = fileToArray(message);
	int keySize = fileToArray(key);
	fprintf(stderr, "message size: %d\n", messageSize);
	fprintf(stderr, "key size: %d\n", keySize);
	fprintf(stderr, "bit size: %d\n", bits('a'));
	
	//Repeat key if it is smaller than the message
	if(keySize < messageSize){
		tiling(key, messageSize, keySize);
		keySize = messageSize;
	}
	
	//Initial set up
	int sum = key[messageSize-1] % messageSize;
	key[0] = rotate(key[0] ^ key[sum], bits(key[messageSize-1]));
	sum = (sum + key[0]) % messageSize;
	
	//Loop through every key and chain them.
	for(int i = 1; i < messageSize; i++){
		key[i] = rotate(key[i] ^ key[sum], bits(key[i-1]));
		sum = (sum + key[i]) % messageSize;
	}
	
	//Encrypt or Decrypt
	for(int i = 0; i < messageSize; i++){
		putchar(message[i] ^ key[i]);
	}
}

/*	Kyle Hinkle
	Rotate letter
*/
unsigned char rotate(unsigned char letter, int count){
	for (int i = 0; i < count; i++){
		if(letter & 1) {
			letter = letter | 1 << 7;
		}
		letter >>= 1;
	}
	return letter;
}

/*	Kyle Hinkle
	Moves bits around
*/
int bits(unsigned char letter){
	int oneBits = 0;
	while (letter > 0){
		if(letter & 1) {
			oneBits++;
		}
		//Moves bit over 1
		letter = letter >> 1;
	}
	return oneBits;
}

/*	Kyle Hinkle
	Read in file to array
*/
int fileToArray(unsigned char message[]){
	int currentChar = getchar();
	int i = 0;
	while(currentChar != 255 && currentChar != -1){
		if(i < SIZE){
			message[i] = currentChar;
			i++;
		}
		currentChar = getchar();
	}
	return i;
}
/*	Kyle Hinkle
	Read in key and adjust it's size
*/
void tiling(unsigned char key[], int arraySize, int originalSize){
	int i = originalSize;
	while(i != arraySize){
		//If it has reached the end of the key loop back around to the start.
		key[i] = key[i % originalSize];
		i++;
	}
}

