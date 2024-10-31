server: main.c
	gcc main.c -o server
	
clean:
	rm -f server
	
run: server
	./server 23034 www
	
debug: main.c
	gcc –ggdb main.c –o server
	
runTest:
	./server.exe 23034 www
