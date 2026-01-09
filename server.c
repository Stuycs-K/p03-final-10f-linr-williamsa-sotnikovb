#include "CommDefs.h"
#include "sqlite3.h"
#include "networking.h"
#include <string.h>
#include <errno.h>

void talkToCli(int client_socket)
{
  int cliSig = -1;
  recv(client_socket, cliSig, sizeof(int), 0);
  if (cliSig==REQLGN)
  {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("example.db", &DB);

    int sendSig = ACCLGN;                         //send a signal accepting login to client
    send(client_socket, sendSig, sizeof(int), 0);
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
  else if (clisig==REQRGST)
  {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("example.db", &DB);

    int sendSig = ACCRGST;
    send(client_socket, sendSig, sizeof(int), 0);
    char unamebuff[256];
    char upwdbuff[256];
    recv(client_socket, unamebuff, 256, 0);
    recv(client_socket, upwdbuff, 256, 0);

    if (!exit)
    {                                             //inserts input values into database
      sqlite3_exec(DB, "CREATE TABLE IF NOT EXISTS users(username TEXT, password TEXT, wins INTEGER)", NULL, 0, NULL);
      char command[256];
      sprintf(command, "INSERT INTO users VALUES('%s', '%s', 0)", unamebuff, upwdbuff);
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

  while (1)
  {
      int client_socket = server_tcp_handshake(listen_socket);
      int id = fork();
      if (id>0)
      {
        close(client_socket);
      }
      else if (!id)
      {
        subserver_logic(client_socket);
      }
      else
      {
        printf("%s\n", strerror(errno));
      }
  }
}
