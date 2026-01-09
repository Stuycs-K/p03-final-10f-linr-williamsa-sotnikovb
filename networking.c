#include "networking.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

void err(int i, char*message){
  if(i < 0){
	  printf("Error: %s - %s\n",message, strerror(errno));
  	exit(1);
  }
}

/*Create and bind a socket.
* Place the socket in a listening state.
* returns the socket descriptor
*/
int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  getaddrinfo(0, "6767", hints, &results);
  //create the socket
  int clientd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);//store the socket descriptor here

  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");

  //bind the socket to address and port
  bind(clientd, results->ai_addr, results->ai_addrlen);
  //set socket to listen state
  listen(clientd, 20);
  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);

  return clientd;
}

/*Accept a connection from a client
 *return the socket descriptor for the new socket connected to the client
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    int client_socket;

    struct sockaddr_storage client_address;
    int size;
    //accept() the client connection
    client_socket = accept(listen_socket, (struct sockaddr *)&client_address, &size);
    printf("Connected to client\n");
    return client_socket;
}


/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {

  //getaddrinfo
  struct addrinfo * hints, * results;
  hints = (struct addrinfo *)calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  getaddrinfo(server_address, "6767", hints, &results);

  int serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  //connect() to the server
  err(connect(serverd, results->ai_addr, results->ai_addrlen), "connecting to server");

  free(hints);
  //freeaddrinfo(results);

  printf("Client connected.\n");
  return serverd;
}

