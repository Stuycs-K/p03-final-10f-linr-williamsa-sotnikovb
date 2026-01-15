#include "CommDefs.h"
#include "networking.h"

void clientLogic(int server_socket){
  printf("Welcome to Battleship 3000\n");
  char loggedin = 0;
  struct usr * self;
  char buff[256];

  while(!loggedin)
  {
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
          printf("You have successfully registered. Please log in.\n");
        }
        else
        {
          printf("Server denied\n");
        }
      }
      else if (!strcmp(buff, "1\n"))
      {
        printf("Please enter your username\n");
        char uname[256];
        char upwd[256];
        fgets(uname, 256, stdin);
        printf("Please enter your password\n");
        fgets(upwd, 256, stdin);
        int sendSig = REQLGN;
        send(server_socket, &sendSig, sizeof(int), 0);
        int recSig = -1;
        recv(server_socket, &recSig, sizeof(int), 0);
        if (recSig==ACCLGN)
        {
          send(server_socket, uname, 256, 0);
          send(server_socket, upwd, 256, 0);
          recv(server_socket, &recSig, sizeof(int), 0);
          if (recSig==CNFRM)
          {
            printf("Welcome, %s.\n", uname);
            recv(server_socket, self, sizeof(struct usr), 0);
            loggedin = 1;
          }
          else if (recSig==DENY)
          {
            printf("Access denied. Please try again or register.\n");
          }
        }
        else
        {
          printf("Server denied\n");
        }
      }
    }
  }

  while(1)
  {
    printf("%s\nWins: %d\nLosses: %d\n", self->name, self->wins, self->losses);
    printf("1. View available players\n2. View leaderboard\n3. Exit");
    fgets(buff, 256, stdin);
    if (!strcmp(buff, "3\n")) exit(0);
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
