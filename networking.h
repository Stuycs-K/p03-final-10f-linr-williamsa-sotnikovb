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
#include <math.h>

#ifndef NETWORKING_H
#define NETWORKING_H
#define PORT "6767"
#define BUFFER_SIZE 1024
const char *inet_ntop2(void *addr, char *buf, size_t size);
struct match * handle_client_data(int s, int listener, fd_set *master, int *fdmax);
void handle_new_connection(int listener, fd_set *master, int *fdmax);
struct match * matchlogic(int socket1, int socket2);
struct match
{
    int pid;
    int socket1;
    int socket2;
};
char* playerList();
struct usr* searchPlayer(int sock);
int searchSocket(char *unm);
int server_setup();
int client_tcp_handshake(char*server_address);
int socket(int domain, int type, int protocol);
int server_tcp_handshake(int listen_socket);
void err(int i, char*message);
void printBoard(int[3][3], int[3][3], int, int);
void clientGameLogic(int server_socket);
struct usr {char name[256]; char pwd[256]; int wins; int losses;};
#endif
