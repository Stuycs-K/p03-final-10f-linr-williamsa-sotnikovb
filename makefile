.PHONY: server clean
compile: networking.o server.o client.o
	@gcc -o server server.o networking.o
	@gcc -o client client.o networking.o
client: client.o
	@./client
client.o: client.c networking.h
	@gcc -Wall -c client.c
server: server.o
	@./server
server.o: server.c networking.h
	@gcc -Wall -c server.c

networking.o: networking.c networking.h
	@gcc -Wall -c networking.c

clean:
	rm -f *.o client server
	rm -f *~
