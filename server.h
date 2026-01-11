#ifndef server_h
#define server_h
int appendDB();
void talkToCli(int client_socket);
void subserver_logic(int client_socket);
void handle_new_connection(int listener, fd_set *master, int *fdmax);
void handle_client_data(int s, int listener, fd_set *master, int fdmax);
const char *inet_ntop2(void *addr, char *buf, size_t size);
#endif