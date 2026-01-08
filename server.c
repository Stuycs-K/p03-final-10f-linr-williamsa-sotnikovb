#include "networking.h"

void matchserverlogic(int client1, int client2){
  while(1){
    #logictba
  }
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();
while(1){
  int client_socket = server_tcp_handshake(listen_socket);
  fd_set desciptors;
  





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
