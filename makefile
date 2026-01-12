.PHONY: server clean
compile: networking.o server.o
	@gcc -o server server.o networking.o

server:
	./server
server.o: server.c networking.h
	@gcc -Wall -c server.c

networking.o: networking.c networking.h
	@gcc -Wall -c networking.c

clean:
	rm -f *.o client server
	rm -f *~
