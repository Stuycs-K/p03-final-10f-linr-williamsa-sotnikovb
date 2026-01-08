#include "networking.h"

void matchserverlogic(int client1, int client2){
  while(1){
    #logictba
  }
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();
  fd_set read_fds;
  int client_socket;
  char buffer[100];

  FD_ZERO(&read_fds);
  //assume this functuion correcly sets up a listening socket
  listen_socket = server_setup();

  //add listen_socket and stdin to the set
  FD_SET(listen_socket, &read_fds);
  //add stdin's file desciptor
  FD_SET(STDIN_FILENO, &read_fds);

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
