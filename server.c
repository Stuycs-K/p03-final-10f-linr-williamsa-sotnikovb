#include "networking.h"

char * rotX(char* n, int x){
  for(int i = 0; i < strlen(n); i++){
    if((*(n+i) >= 'a') && (*(n+i) <= 'z')){
      *(n+i) = 'a' + ((*(n+i) + x - 'a') % 26);
    }
    if((*(n+i) >= 'A') && (*(n+i) <= 'Z')){
      *(n+i) = 'Z' + ((*(n+i) + x - 'A') % 26);
    }
  }
  return n;
}

void subserver_logic(int client_socket){
  while(1){
    char buff[BUFFER_SIZE];
    if(0==recv(client_socket, buff, sizeof(buff), MSG_WAITALL)){
      exit(0);
    }
    rotX(buff, 13);
    send(client_socket, buff, sizeof(buff), 0);
  }
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();
while(1){
  int client_socket = server_tcp_handshake(listen_socket);
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
