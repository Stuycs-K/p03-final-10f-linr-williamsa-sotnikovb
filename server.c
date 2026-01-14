#include "CommDefs.h"
#include "networking.h"
#include "server.h"
#define TRUE 1
#define FALSE 0
#define MAX_CLIENTS FD_SETSIZE
#define MAX_NUMBOARDS 10
#include <string.h>
#include <errno.h>


/*
We will have a single main server that manages incoming connections, and manages match requests.
We listen on an open, "master_socket", for new connections and listen on every other socket for match requests. This is achieved using the select function. - note that select is blocking
When a client connects we add them to our select list and continue.
When a client requests to play another client on the list, we must write to the other client socket, requiring us to have both reading and writing ports open.
*/

//main server code
int main(int argc, char *argv[] ) {
  fd_set master;    // master file descriptor list
  fd_set read_fds;  // temp file descriptor list for select()
  int fdmax;        // maximum file descriptor number
  //struct timeval tv;
  //tv.tv_sec = 2; // select function will continue every 2 sec

  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  int listen_socket = server_setup();
  struct board * boardarray[MAX_NUMBOARDS]; // may need to make sure this is null? and initialize?
  //add listen_socket to the set
  FD_SET(listen_socket, &master);
  fdmax = listen_socket;
  while(1){ //main loop
    read_fds = master;
    int retval = 0;
    struct board * newboard = NULL;
    if(select(fdmax+1,&read_fds, NULL, NULL, NULL) == -1){
      perror("select");
      exit(16);
    }
    for(int i = 0; i <= fdmax; i++){
      if(FD_ISSET(i, &read_fds)) {
        if(i == listen_socket)
          handle_new_connection(i, &master, &fdmax);
        else
          newboard = handle_client_data(i, listen_socket, &master, &fdmax);
      }
    }
    if(newboard != NULL){
      for(int j = 0; j < MAX_NUMBOARDS; j++){
        if(boardarray[j] != NULL){
          boardarray[j] = newboard;
          break;
        }
      }
    }
    for(int k = 0; k < MAX_NUMBOARDS; k++){
      retval = 0;
      waitpid(boardarray[j]->pid, &retval, WNOHANG);
      if(retval != 0){
        // code tba?
      }
    }
  }
}

int appendDB(struct usr * u)
{
  struct usr * temp = searchDB(u->name, u->pwd);
  if (!temp)
  {
    int w_file = open("./userdata.ussv", O_WRONLY|O_APPEND, 0);
    write(w_file, u, sizeof(struct usr));
  }
  else free(temp);
  return -1;
}
//Combined BEEJ's Code w/ lab-16 server_tcp_handshake implementation
void handle_new_connection(int listener, fd_set *master, int *fdmax){
  int newfd = server_tcp_handshake(listener);
  if (newfd == -1) {
    perror("accept");
  }
  else{
    FD_SET(newfd, master); // add to master set
    if (newfd > *fdmax) {  // keep track of the max
    *fdmax = newfd;
    }
  }
  printf("server connected\n");
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
  }
  close(r_file);
  return NULL;
}

struct * board handle_client_data(int s, int listener, fd_set *master, int *fdmax){
  char buf[256];    // buffer for client data
  int cliSig = -1;
  int nbytes;
  // handle data from a client
  if((nbytes = recv(s, &cliSig, sizeof cliSig, 0)) <= 0){ // got error or connection closed by client
    if(nbytes == 0){ // connection closed
      printf("selectserver: socket %d hung up\n", s);
    }
    else{
      perror("recv");
    }
    close(s); // bye!
    FD_CLR(s, master); // remove from master set
    if(s == *fdmax){
      while (*fdmax >= 0 && !FD_ISSET(*fdmax, master)){
      (*fdmax)--;
      }
    }
  }
  else{
    buf[nbytes] = '\0';
    if(/*conditions tba*/){
      return matchlogic(socket1, socket2);
    }
        // we got some data from a client
        // to implement how we handle this data
        // - if its a name of another client then we send request to that other client, should also send a "request sent" to first client
        // - if its an acceptance we should make the match server and remove both clients from our "online" list
    if (cliSig==REQLGN)
    {
      int sendSig = ACCLGN;                         //send a signal accepting login to client
      send(s, &sendSig, sizeof(int), 0);
      char unamebuff[256];
      char upwdbuff[256];
      recv(s, unamebuff, 256, 0);       //await username and pwd input
      recv(s, upwdbuff, 256, 0);
      struct usr * temp = searchDB(unamebuff, upwdbuff);
      if (!temp)
      {
        sendSig = DENY;
        send(s, &sendSig, sizeof(int), 0);
      }
      else
      {
        sendSig = CNFRM;
        send(s, &sendSig, sizeof(int), 0);
        printf("User %s connected\n", temp->name);
        //send(s, temp, sizeof(struct usr), 0);
      }
    }
    else if (cliSig==REQRGST)
    {
      int sendSig = ACCRGST;
      send(s, &sendSig, sizeof(int), 0);
      char unamebuff[256];
      char upwdbuff[256];
      recv(s, unamebuff, 256, 0);
      recv(s, upwdbuff, 256, 0);
      struct usr * newAcc = (struct usr *)calloc(1, sizeof(struct usr));
      strcpy(newAcc->name, unamebuff);
      strcpy(newAcc->pwd, upwdbuff);
      newAcc->wins = 0;
      newAcc->losses = 0;
      appendDB(newAcc);
      free(newAcc);
    }
  }
}

struct * board matchlogic(int socket1, int socket2){
  int subpid = fork();
  if(subpid == 0){
    struct board * newboard = (struct board *) calloc(sizeof(board));
    newboard->pid = getpid();
    newboard->socket1 = socket1;
    newboard->socket2 = socket2;
  }
  else{
    return subpid;
  }
}




/*straight BEEJ's code for converting a socket into an IP address string
//this is just - for the moment at least - for being able to bug test and tell who's who
const char *inet_ntop2(void *addr, char *buf, size_t size){
  struct sockaddr_storage *sas = addr;
  struct sockaddr_in *sa4;
  struct sockaddr_in6 *sa6;
  void *src;
  switch (sas->ss_family) { //whats a switch? fancy if else?
    case AF_INET:
      sa4 = addr;
      src = &(sa4->sin_addr);
      break;
    case AF_INET6:
      sa6 = addr;
      src = &(sa6->sin6_addr);
      break;
    default:
      return NULL;
    }
    return inet_ntop(sas->ss_family, src, buf, size);
}
*/
