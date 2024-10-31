all: client server

client: main.c
	gcc main.c -o client

server: server.c
	gcc server.c -o server
	
clean:
	rm -f client server
	
run1: client
	./client

run2: server
	./server
	
debug: main.c
	gcc –ggdb main.c –o client
