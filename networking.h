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


#ifndef NETWORKING_H
#define NETWORKING_H
#define PORT "6767"
#define BUFFER_SIZE 1024

int server_setup();
int client_tcp_handshake(char*server_address);
int socket(int domain, int type, int protocol);
int server_tcp_handshake(int listen_socket);
struct usr {char *name; char *pwd; int wins; int losses;};
#endif
