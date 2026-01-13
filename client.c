#include "networking.h"

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

void clientLogic(int server_socket){
  while (1){
  char buffer[256];
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

  }
  return;
}

int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  while(1){
  int server_socket = client_tcp_handshake(IP);
  printf("client connected.\n");
  clientLogic(server_socket);
  close(server_socket);
  exit(0);
}
}
