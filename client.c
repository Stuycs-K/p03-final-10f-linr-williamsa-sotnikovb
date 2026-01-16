#include "networking.h"
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
        uname[strlen(uname)-1] = NULL;
        upwd[strlen(upwd)-1] = NULL;

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
    printf("1. View available players\n2. Connect to player\n3. View leaderboard\n4. Exit");
    fgets(buff, 256, stdin);
    if (!strcmp(buff, "4\n")) exit(0);
    else if (!strcmp(buff, "3\n"))
    {
      int sendSig = REQLDBRD;
      send(server_socket, &sendSig, sizeof(int), 0);

      int size = 0;
      recv(server_socket, &size, sizeof(int));
      struct usr * * players = (struct usr **)calloc(size, 1);
      recv(server_socket, players, size);
      printf("LEADERBOARD:\n\n");
      for (size_t i = 0; i < size/sizeof(struct usr); i++)
      {
        printf("%d. %256s - %2d wins - %2d losses", i+1, players[i]->name, players[i]->wins, players[i]->losses);
      }
    }
  }
}

void printBoard(int myBoard[3][3], int oppBoard[3][3], int x, int y){
  printf("Opponent guessed (%d, %d)\n", x, y);
  printf("Your Board: \n");
  printf("   0  1  2\n");
  for(int i = 0; i < 3; i++){
    for (int b = 0; b < 4; b++){
      if (b == 0){
        printf("%d ", b);
      }
      else{
        printf("%d ", myBoard[i][b - 1]);
      }
    }
    printf("\n");
  }
  printf("This is your opponent's board: \n");
  printf("   0  1  2\n");
  for(int i = 0; i < 3; i++){
    for (int b = 0; b < 4; b++){
      if (b == 0){
        printf("%d ", b);
      }
      else{
        if (oppBoard[i][b-1] == -1 || oppBoard[i][b-1] == 2){
          printf("%d ", oppBoard[i][b - 1]);
        }
        else{
        printf("%s ", "*");
      }
    }
    }
    printf("\n");
  }
}

void clientGameLogic(int server_socket){
  while (1){
  char buffer[256];
  int x;
  int y;
  printf("Insert the coordinates you wish to check in x y form: ");
  char * s = fgets(buffer, sizeof(buffer), stdin);
  if (s == NULL){
    printf("Client exits because of CTRL-D\n");
    break;
  }
  int bytes = write(server_socket, buffer, 256);
  if (bytes == -1){
    err(server_socket, "write failed");
  }
  int board[2][3][3];
  bytes = read(server_socket, buffer, sizeof(buffer));
  if (bytes == -1){
    err(server_socket, "read failed");
  }
  sscanf(buffer, "%d %d", &x, &y);
  bytes = read(server_socket, board, sizeof(board));
  if (bytes == -1){
    err(server_socket, "read failed");
  }
  int myBoard[3][3];
  int oppBoard[3][3];
  memcpy(myBoard, board[0], sizeof(board[0]));
  memcpy(oppBoard, board[1], sizeof(board[1]));
  printBoard(myBoard, oppBoard, x, y);
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
