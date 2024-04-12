onetimepad: onetimepad.c
	gcc onetimepad.c -o onetimepad -Wall

clean:
	rm -f onetimepad
	
run: 
	cat message1.txt delimiter.txt key1.txt | ./onetimepad > cipher.txt
	
run2: 
	cat message2.txt delimiter.txt key2.txt | ./onetimepad > cipher.txt
