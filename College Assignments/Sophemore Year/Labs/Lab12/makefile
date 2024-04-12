checksum: main.c
	gcc main.c -o checksum
	
clean:
	rm -f checksum
	
run: checksum
	./checksum makefile
	
debug: main.c
	gcc –ggdb main.c –o checksum
	
runTest:
	sum -s makefile
