#ifndef server_h
#define server_h
int appendDB(struct usr * u);
struct usr * searchDB(char *unm, char *pwd);

void talkToCli(int client_socket);
void subserver_logic(int client_socket);
#endif
