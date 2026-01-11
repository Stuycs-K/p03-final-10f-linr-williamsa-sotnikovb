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
  struct addrinfo hints;
  struct addrinfo *servinfo;  // will point to the results
  memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
  getaddrinfo(NULL, PORT, &hints, &servinfo);

  //create the socket
  int clientd = socket(servinfo->ai_family, servinfo-> ai_socktype, servinfo-> ai_protocol);//store the socket descriptor here
  err(clientd, "socket error");
   //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");

  //bind the socket to address and port
  int bindres = bind(clientd, servinfo->ai_addr, servinfo->ai_addrlen);
  err(bindres, "bind error");
  //set socket to listen state
  int listened = listen(clientd, 10);
  err(listened, "listening error");
  //free the structs used by getaddrinfo

  freeaddrinfo(servinfo);
  return clientd;
}

/*Accept a connection from a client
 *return the socket descriptor for the new socket connected to the client
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    int client_socket;
    socklen_t sock_size;
    struct sockaddr_in client_address;
    sock_size = sizeof(client_address);
    client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
    err(client_socket, "accept error");
    //accept() the client connection
    return client_socket;
}


/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {

  //getaddrinfo
  struct addrinfo * hints, * results;//results is allocated in getaddrinfo
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = 0; //only needed on server
  getaddrinfo(server_address, PORT, hints, &results);  //Server sets node to NULL

  //create socket
  int serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  //bind(serverd, results->ai_addr, results->ai_addrlen);

  //DO STUFF

  //store the socket descriptor here
  //create the socket
  connect(serverd, results -> ai_addr, results -> ai_addrlen);
  //connect() to the server

  freeaddrinfo(results);

  return serverd;
}
