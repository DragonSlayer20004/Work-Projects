contacts: lab9.h contacts.c
	gcc contacts.c -Wall -o contacts
	
clean:
	rm -f contacts
	
run: contacts
	./contacts
	
debug: contacts.c lab9.h
	gcc –ggdb contacts.c –o contacts
