#include "CommDefs.h"
#include "networking.h"
#include "server.h"
#define TRUE 1
#define FALSE 0
#define MAX_CLIENTS FD_SETSIZE
#include <string.h>
#include <errno.h>

void talkToCli(int client_socket)
{
  int cliSig = -1;
  recv(client_socket, &cliSig, sizeof(int), 0);
  if (cliSig==REQLGN)
  {
    int sendSig = ACCLGN;                         //send a signal accepting login to client
    send(client_socket, &sendSig, sizeof(int), 0);
    char unamebuff[256];
    char upwdbuff[256];
    recv(client_socket, unamebuff, 256, 0);       //await username and pwd input
    recv(client_socket, upwdbuff, 256, 0);
    struct usr * temp = searchDB(unamebuff, upwdbuff);
    if (!temp)
    {
      sendSig = DENY;
      send(client_socket, &sendSig, sizeof(int), 0);
    }
    else
    {
      sendSig = CNFRM;
      send(client_socket, &sendSig, sizeof(int), 0);
      send(client_socket, temp, sizeof(struct usr), 0);
    }
  }
  else if (cliSig==REQRGST)
  {
    int sendSig = ACCRGST;
    send(client_socket, &sendSig, sizeof(int), 0);
    char unamebuff[256];
    char upwdbuff[256];
    recv(client_socket, unamebuff, 256, 0);
    recv(client_socket, upwdbuff, 256, 0);
    struct usr * newAcc = (struct usr *)calloc(1, sizeof(struct usr));
    newAcc->name = unamebuff;
    newAcc->pwd = upwdbuff;
    appendDB(newAcc);
    free(newAcc);
  }
}

int appendDB(struct usr * u);
{
  struct usr * temp = searchDB(u->name, u->pwd);
  if (!temp)
  {
    int w_file = open("./userdata.ussv", O_WRONLY|O_APPEND, 0);
    write(w_file, u, sizeof(u));
  }
  free(temp);
  return -1;
}

struct usr * searchDB(char *unm, char *pwd)
{
  int r_file = open("./userdata.ussv", O_RDONLY, 0);
  if (r_file == -1)
  {
    close(r_file);
    return NULL;
  }
  struct usr * out = (struct usr *)calloc(1, sizeof(struct usr));
  while (read(r_file, out, sizeof(struct usr)))
  {
    if (!strcmp(out->name, unm) && !strcmp(out->pwd, pwd))
    {
      close(r_file);
      return out;
  }
  close(r_file);
  return NULL;
}
