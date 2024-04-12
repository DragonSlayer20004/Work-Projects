#include <stdio.h>

/*function declaration*/
int lazyCatererSequence();
int primeNumbers();
int primeChecker();
int fibonacci();
int collatzStoppingTimes();
void happyNumbers();

/* Kyle Hinkle
Main method that runs all the sequences
*/
int main() {
	int countAmount = 50;
	lazyCatererSequence(countAmount);
	printf("\n");
	primeNumbers(countAmount);
	printf("\n");
	fibonacci(countAmount);
	printf("\n");
	collatzStoppingTimes(countAmount);
	printf("\n");
	happyNumbers(countAmount);
	printf("\n");
	return 0;
}	

/* Kyle Hinkle
This method does the LazyCaterer Sequence 
 */
int lazyCatererSequence(int countAmount) {
	int i;
	int value = 0;
	printf("Lazy Caterer's Sequence:\n");
	for (i = 0; i < countAmount; i++){
		value = (i*i + i + 2)/2;
		printf("%d ", value);
	}
	printf("\n");
	return 0;
}
/* Kyle Hinkle
This method does the primeNumbers Sequence 
 */
int primeNumbers(int countAmount) {
	int i;
	int totalPrimes = 0;
	printf("Prime Numbers:\n");
	for (i = 2; totalPrimes < countAmount; i++){
		if(primeChecker(i) == 1){
			printf("%d ", i);
			totalPrimes++;
		}
	}
	printf("\n");
	return 0;
}

/* Kyle Hinkle
This method checks if a number is prime
 */
int primeChecker(int number) {
	int i;
	int isPrime = 1;
	for (i = 2; i < number/2 + 1 & isPrime == 1; i++){
		if(number % i == 0){
			isPrime = 0;
		}
	}
	
	return isPrime;
}

/* Kyle Hinkle
This method does the Fibonacci Sequence 
 */
int fibonacci(int countAmount) {
	int i;
	long long value1 = 0;
	long long value2 = 1;
	long long temp = 0;
	printf("Fibonacci Sequence:\n");
	for (i = 2; i < countAmount + 2; i++){
		temp = value1;
		value1 = value1 + value2;
		value2 = temp;
		printf("%lld ", value1);
	}
	printf("\n");
	return 0;
}

/* Ibrahim Hashi
This method does the Collatz Stopping Times
 */
int collatzStoppingTimes(int countAmount) {
	int i;
	int n;
	printf("Collatz Stopping Times:\n");
	for(i = 1; i < countAmount + 1; i++) {
		int total = 0;
		n = i;
		while(n != 1) {	
			total++;	
			if(n % 2 == 0) {
				n = n / 2;
			}
			else {
				n = (n * 3) + 1;
			}
		}
		printf("%d ", total);
	}
	printf("\n");
	return 0;
}


/* Ibrahim Hashi
This method does the Happy Numbers
 */
void happyNumbers(int countAmount) { 
    int i = 1;
    int numbers = 0; 
	printf("Happy Numbers:\n");
	while(numbers < countAmount) {
		int variable = i;
		while(variable != 1 && variable != 4){
			int sumDigit = 0;
			while(variable != 0) {
				int digit = variable % 10;
				digit = digit * digit;
				sumDigit += digit;
				variable = variable / 10;
			}
			variable = sumDigit;	
		}
		if(variable == 1) {
			printf("%d ", i);
			numbers++;
		}
		i++;
	}	
 }

