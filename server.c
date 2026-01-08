#include "CommDefs.h"
#include <sqlite3.h>

void talkToCli(int client_socket)
{
  int cliSig = -1;
  recv(client_socket, cliSig, sizeof(int), 0);
  if (cliSig==REQLGN)
  {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("example.db", &DB);

    int sendSig = ACCLGN;
    send(client_socket, sendSig, sizeof(int), 0);
    char unamebuff[256];
    char upwdbuff[256];
    recv(client_socket, unamebuff, 256, 0);
    recv(client_socket, upwdbuff, 256, 0);

    if (!exit)
    {
      
    }
    sqlite3_close(DB);
  }
}
