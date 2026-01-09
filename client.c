#include "networking.h"

void printBoard(int[3][3] myBoard, int[3][3] oppBoard, int x, int y){
  printf("Opponent guessed (%d, %d)\n");
  printf("Your Board: "\n);
  printf("   0  1  2\n");
  for(int i = 0; i < 3; i++){
    for (int x = 0; x < 4; x++){
      if (x == 0){
        printf("%s ", x);
      }
      else{
        printf("%s ", myBoard[i][x - 1]);
      }
    }
    printf("\n");
  }
  printf("This is your opponent's board: \n");
  printf("   0  1  2\n");
  for(int i = 0; i < 3; i++){
    for (int x = 0; x < 4; x++){
      if (x == 0){
        printf("%s ", x);
      }
      else{
        if (oppBoard[i][x-1] == -1 || oppBoard[i][x-1] == 2){
          printf("%s ", oppBoard[i][x - 1]);
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
  while(1){
  char buffer[BUFFER_SIZE];
  printf("What coordinates do you want to guess: ");
  char * s = fgets(buffer, sizeof(buffer), stdin);
  if (s == NULL){
    printf("Client exits because of CTRL-D\n");
    break;
  }
  for(int i = 0; i < BUFFER_SIZE; i++){
    if(buffer[i] == '\n'){
      buffer[i] = '\0';
    }
    if (buffer[i] == '\r'){
      buffer[i] = '\0';
    }
  }
  int bytes = write(server_socket, buffer, strlen(buffer));
  err(bytes, "client write failed\n");
  printf("Client writes %s to Server\n", buffer);
  bytes = read(server_socket, buffer, BUFFER_SIZE);
  if (bytes == 0){
    printf("Client quits because read returns 0 bytes\n");
    break;
  }
  err(bytes, "client read failed\n");
  printf("Client receives the modified string: %s\n", buffer);
}
}

int main(int argc, char *argv[] ) {
  signal(SIGINT, sighandler);
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
