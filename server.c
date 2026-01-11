#include "networking.h"
#define TRUE 1
#define FALSE 0
#define PORT 6767
#define MAX_CLIENTS FD_SETSIZE
void matchserverlogic(int client1, int client2){
  while(1){
    //logictba
  }
}

/*
We will have a single main server that manages incoming connections, and manages match requests.
We listen on an open, "master_socket", for new connections and listen on every other socket for match requests. This is achieved using the select function. - note that select is blocking
When a client connects we add them to our select list and continue.
When a client requests to play another client on the list, we must write to the other client socket, requiring us to have both reading and writing ports open.
*/


int main(int argc, char *argv[] ) {
  fd_set master;    // master file descriptor list
  fd_set read_fds;  // temp file descriptor list for select()
  int fdmax;        // maximum file descriptor number
  //struct timeval tv;
  //tv.tv_sec = 2; // select function will continue every 2 sec
  int listener = server_setup();
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  int client_socket;
  char buffer[100];

  FD_ZERO(&read_fds);
  //assume this functuion correcly sets up a listening socket
  listen_socket = server_setup();
  //add listen_socket and stdin to the set
  FD_SET(listen_socket, &master);
  fdmax = listen_socket;

  while(0){ //main loop
    read_fds = master;
    if(select(fdmax+1,&read_fds, NULL, NULL, NULL) == -1){
      perror("select");
      exit(16);
    }
    for(int i = 0; i <= fdmax; i++){
      if(FD_ISSET(i, &read_fds)) {
        if(i == listener)
          handle_new_connection(i, &master, &fdmax);
        else
          handle_client_data(i, listener, &master, fdmax);
      }
    }
  }
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
    printf("selectserver: new connection from %s on socket %d\n", inet_ntop2(&remoteaddr, remoteIP, sizeof remoteIP), newfd);
  }
}

void handle_client_data(int s, int listener, fd_set *master, int fdmax){
  char buf[256];    // buffer for client data
  int nbytes;
  // handle data from a client
  if((nbytes = recv(s, buf, sizeof buf, 0)) <= 0) { // got error or connection closed by client
    if(nbytes == 0) { // connection closed
      printf("selectserver: socket %d hung up\n", s);
    }
    else{
      perror("recv");
    }
    close(s); // bye!
    FD_CLR(s, master); // remove from master set
  }
  else{
        // we got some data from a client
        // to implement how we handle this data
        // - if its a name of another client then we send request to that other client, should also send a "request sent" to first client
        // - if its an acceptance we should make the match server and remove both clients from our "online" list
  }
}

  //add stdin's file desciptor
  //FD_SET(STDIN_FILENO, &read_fds);

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
