#include "networking.h"
#include& lt; stdio.h & gt;
#include& lt; string.h & gt; //strlen
#include& lt; stdlib.h & gt;
#include& lt; errno.h & gt;
#include& lt; unistd.h & gt; //close
#include& lt; arpa / inet.h & gt; //close
#include& lt; sys / types.h & gt;
#include& lt; sys / socket.h & gt;
#include& lt; netinet / in.h & gt;
#include& lt; sys / time.h & gt; //FD_SET, FD_ISSET, FD_ZERO macros
#define TRUE 1
#define FALSE 0
#define PORT 6767

void matchserverlogic(int client1, int client2){
  while(1){
    #logictba
  }
}

/*
We will have a single main server that manages incoming connections, and manages match requests.
We listen on an open, "master_socket", for new connections and listen on every other socket for match requests. This is achieved using the select function.
When a client connects we add them to our select list and continue.
When a client requests to play another client on the list, we must write to the other client socket, requiring us to have both reading and writing ports open.
*/
int main(int argc, char *argv[] ) {
  fd_set master;    // master file descriptor list
  fd_set read_fds;  // temp file descriptor list for select()
  int fdmax;        // maximum file descriptor number

  int listener = server_setup();
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  FD_SET
  int client_socket;
  char buffer[100];

  FD_ZERO(&read_fds);
  //assume this functuion correcly sets up a listening socket
  listen_socket = server_setup();

  //add listen_socket and stdin to the set
  FD_SET(listen_socket, &master);
  fdmax = listen_socket;
  //add stdin's file desciptor
  //FD_SET(STDIN_FILENO, &read_fds);

  //listen socket is larger than STDIN_FILENO, so listen_socket+1 is the 1 larger than max fd value.
  int i = select(listen_socket+1, &read_fds, NULL, NULL, NULL);

  //if standard in, use fgets
  if (FD_ISSET(STDIN_FILENO, &read_fds)) {
    fgets(buffer, sizeof(buffer), stdin);
  }
  //if socket, accept the connection
  //assume this function works correctly
  if (FD_ISSET(listen_socket, &read_fds)) {
    client_socket = server_connect(listen_socket);
  }
while(1){
  int client_socket = server_tcp_handshake(listen_socket);
  fd_set desciptors;
  FD_ZERO( &desciptors );





  int child = fork();
  if(!child){
    close(listen_socket);
    subserver_logic(client_socket);
    close(client_socket);
    exit(0);
  }
  close(client_socket);
}

}
