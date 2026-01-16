.PHONY: clean compile run
compile server client: networking.o server.o client.o networking.h CommDefs.h
	@gcc -o server server.o networking.o -lm
	@gcc -o client client.o networking.o
networking.o: networking.c networking.h
	@gcc -c networking.c
server.o: server.c networking.h CommDefs.h
	@gcc -c server.c -lm
client.o: client.c networking.h CommDefs.h
	@gcc -c client.c
clean:
	@rm -f *.o networking server client
