<<<<<<< HEAD
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

=======
.PHONY: clean compile run
compile server client: networking.o server.o client.o networking.h CommDefs.h
	@gcc -o server server.o networking.o
	@gcc -o client client.o networking.o
>>>>>>> main
networking.o: networking.c networking.h
	@gcc -c networking.c
server.o: server.c networking.h CommDefs.h
	@gcc -c server.c
client.o: client.c networking.h CommDefs.h
	@gcc -c client.c
clean:
	@rm -f *.o networking server client
