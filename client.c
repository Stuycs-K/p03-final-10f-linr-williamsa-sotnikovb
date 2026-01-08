#include "networking.h"

void clientLogic(int server_socket){
  printf("Welcome to Battleship 3000\nPlease enter your username\n");
  char buffer[256];
  fgets()
  while (1)
  {

  }
}

int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  int server_socket = client_tcp_handshake(IP);
  clientLogic(server_socket);
}
