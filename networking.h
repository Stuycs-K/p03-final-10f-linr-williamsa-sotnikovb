#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/queue.h> // essentially linked list implementation

#ifndef NETWORKING_H
#define NETWORKING_H
#define PORT "6767"
#define BUFFER_SIZE 1024
const char *inet_ntop2(void *addr, char *buf, size_t size);
void handle_client_data(int s, int listener, fd_set *master, int *fdmax);
void handle_new_connection(int listener, fd_set *master, int *fdmax);
struct board
{
    int pid;
    int socket1;
    int socket2;
};
int server_setup();
int client_tcp_handshake(char*server_address);
int socket(int domain, int type, int protocol);
int server_tcp_handshake(int listen_socket);
#endif
