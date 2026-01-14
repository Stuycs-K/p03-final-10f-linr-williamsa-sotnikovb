#include "CommDefs.h"
#include "networking.h"

void clientLogic(int server_socket){
  printf("Welcome to Battleship 3000\n");
  char loggedin = 0;
  while(!loggedin)
  {
    char buff[256];
    printf("Enter 1 for login. Enter 2 for registration.\n");
    if (fgets(buff, 256, stdin))
    {
      if (!strcmp(buff, "2\n"))
      {
        printf("Please enter a username\n");
        char uname[256];
        char upwd[256];
        fgets(uname, 256, stdin);
        printf("Please enter a password\n");
        fgets(upwd, 256, stdin);
        int sendSig = REQRGST;
        send(server_socket, &sendSig, sizeof(int), 0);
        int recSig = -1;
        recv(server_socket, &recSig, sizeof(int), 0);
        if (recSig==ACCRGST)
        {
          send(server_socket, uname, 256, 0);
          send(server_socket, upwd, 256, 0);
          loggedin = 1;
        }
        else
        {
          printf("Server denied\n");
        }
      }
    }
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
