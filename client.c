#include "CommDefs.h"

void clientLogic(int server_socket){
  printf("Welcome to Battleship 3000\n");
  char loggedin = 0;
  while(!loggedin)
  {
    printf("Please enter your username\n");
    char uname[256];
    char upwd[256];
    if (fgets(uname, 256, stdin))
    {
      printf("Please enter your password\n");
      if (fgets(upwd, 256, stdin))
        if ()
    }
  }
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
