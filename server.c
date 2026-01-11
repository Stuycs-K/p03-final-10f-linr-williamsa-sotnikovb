<<<<<<< HEAD
#include "CommDefs.h"
#include "sqlite3.h"
#include "networking.h"
#include <string.h>
#include <errno.h>

void talkToCli(int client_socket)
{
  int cliSig = -1;
  recv(client_socket, &cliSig, sizeof(int), 0);
  if (cliSig==REQLGN)
  {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("example.db", &DB);

    int sendSig = ACCLGN;                         //send a signal accepting login to client
    send(client_socket, &sendSig, sizeof(int), 0);
    char unamebuff[256];
    char upwdbuff[256];
    recv(client_socket, unamebuff, 256, 0);       //await username and pwd input
    recv(client_socket, upwdbuff, 256, 0);

    if (!exit)
    {
      sqlite3_exec(DB, "CREATE TABLE IF NOT EXISTS users(username TEXT, password TEXT, wins INTEGER)", NULL, 0, NULL);
    }
    sqlite3_close(DB);
  }
  else if (cliSig==REQRGST)
  {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("example.db", &DB);

    int sendSig = ACCRGST;
    send(client_socket, &sendSig, sizeof(int), 0);
    char unamebuff[256];
    char upwdbuff[256];
    recv(client_socket, unamebuff, 256, 0);
    recv(client_socket, upwdbuff, 256, 0);

    if (!exit)
    {                                             //inserts input values into database
      sqlite3_exec(DB, "CREATE TABLE IF NOT EXISTS users(username TEXT, password TEXT, wins INTEGER)", NULL, 0, NULL);
      char command[548];
      sprintf(command, "INSERT INTO users VALUES('%256s', '%256s', 0)", unamebuff, upwdbuff);
      sqlite3_exec(DB, command, NULL, NULL, NULL);
    }
    sqlite3_close(DB);
  }
}

void subserver_logic(int client_socket){
  talkToCli(client_socket);
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();
  fd_set read_fds;
  int client_socket;
  char buffer[100];

  FD_ZERO(&read_fds);
  //assume this functuion correcly sets up a listening socket

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
