#include "CommDefs.h"
#include "networking.h"
#include "server.h"
#define TRUE 1
#define FALSE 0
#define MAX_CLIENTS FD_SETSIZE
#define MAX_NUMMATCHES 10
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
  fd_set activemaster;
  fd_set read_fds;  // temp file descriptor list for select()
  int fdmax;        // maximum file descriptor number
  //struct timeval tv;
  //tv.tv_sec = 2; // select function will continue every 2 sec
  remove("activeplayers.ussv");
  FD_ZERO(&activemaster);
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  int listen_socket = server_setup();
  struct match * matcharray[MAX_NUMMATCHES]; // may need to make sure this is null? and initialize?
  for (int m = 0; m < MAX_NUMMATCHES; m++) {
    matcharray[m] = calloc(1, sizeof *matcharray[m]);
}
  //add listen_socket to the set
  FD_SET(listen_socket, &master);
  fdmax = listen_socket;
  activemaster = master;
  while(1){ //main loop
    read_fds = activemaster;
    int retval = 0;
    struct match * newmatch = NULL;
    if(select(fdmax+1,&read_fds, NULL, NULL, NULL) == -1){
      perror("select");
      exit(0);
    }
    for(int i = 0; i <= fdmax; i++){
      if(FD_ISSET(i, &read_fds)) {
        printf("%d is set\n", i);
        if(i == listen_socket){
          handle_new_connection(i, &activemaster, &fdmax);
          master = activemaster;
        }
        else{
          newmatch = handle_client_data(i, listen_socket, &activemaster, &fdmax);
          if(newmatch != NULL){
            FD_CLR(newmatch->socket1, &activemaster);
            FD_CLR(newmatch->socket2, &activemaster);
          }
        }
      }
    }
    if(newmatch != NULL){
      for(int j = 0; j < MAX_NUMMATCHES; j++){
        if(matcharray[j] != NULL){
          matcharray[j] = newmatch;
          break;
        }
      }
    }
    for(int k = 0; k < MAX_NUMMATCHES; k++){
      if(matcharray[k] != NULL){
        retval = 0; // retval should give either the winner's socket or if one exited - but how would it tell which one?
        int matchpid = 0;
        matchpid = waitpid(matcharray[k]->pid, &retval, WNOHANG);
        //needs to unblock from sockets - may need to for loop?
        if(retval != 0){
          for(int l = 0; l < MAX_NUMMATCHES; l++){
            if(matchpid == matcharray[l]->pid){
              FD_SET(matcharray[l]->socket1, &activemaster);
              FD_SET(matcharray[l]->socket2, &activemaster);
            }
          }
        // code tba - the matcharray should be updated - removing this index
        // the leaderboard should be updated
        }
        matcharray[k] = NULL;
      }
    }
  }
}

int compareUsrs(const void * u1, const void * u2)
{
  return strcmp(((const struct usr *)u1)->name, ((const struct usr *)u2)->name);
}

struct usr * * getPlayers()
{
  int r_file = open("./userdata.ussv", O_RDONLY, 0);
  if (r_file == -1)
  {
    close(r_file);
    return NULL;
  }

  FILE* fp = fopen("./userdata.ussv", "r");
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  size/=sizeof(struct usr);
  fseek(fp, 0, SEEK_SET);
  struct usr * * out = (struct usr * *)calloc(size, sizeof(struct usr));
  for (int i = 0; i < size; i++)
  {
    read(r_file, out[i], sizeof(struct usr));
    strcpy(out[i]->pwd, "");
  }
  qsort(out, size, sizeof(struct usr), compareUsrs);
  close(r_file);
  return out;
}

int appendDB(struct usr * u)
{
  struct usr * temp = searchDB(u->name, u->pwd);
  if (!temp)
  {
    int w_file = open("./userdata.ussv", O_WRONLY|O_APPEND, 0);
    write(w_file, u, sizeof(struct usr));
    close(w_file);
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
    printf("socket on %d connected to server", newfd);
    }
  }
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

int searchSocket(char *unm)
{
  int r_file = open("./activeplayers.ussv", O_RDONLY, 0);
  int socket = -1;
  if (r_file == -1)
  {
    close(r_file);
    return -1;
  }
  struct usr * out = (struct usr *)calloc(1, sizeof(struct usr));
  while (read(r_file, out, sizeof(struct usr)))
  {
    if (!strcmp(out->name, unm))
    {
      read(r_file, &socket, sizeof(int)+1);
      close(r_file);
      return socket;
    }
    read(r_file, &socket, sizeof(int)+1);
  }
  close(r_file);
  return -1;
}

struct usr* searchPlayer(int sock)
{
  int r_file = open("./activeplayers.ussv", O_RDONLY, 0);
  int socket = -1;
  if (r_file == -1)
  {
    perror("open");
    return NULL;
  }
  struct usr * out = (struct usr *)calloc(1, sizeof(struct usr));
  while (read(r_file, out, sizeof(struct usr)))
  {
    read(r_file, &socket, sizeof(int));
    printf("socket: %d\n", socket);
    if (sock == socket)
    {
      close(r_file);
      return out;
    }
  }
  close(r_file);
  printf("hereya\n");
  return NULL;
}

char* playerList()
{
  int r_file = open("./userdata.ussv", O_RDONLY, 0);
  if (r_file == -1)
  {
    close(r_file);
    return NULL;
  }
  struct usr * out = (struct usr *)calloc(1, sizeof(struct usr));
  char *str = malloc(5000);
  str[0] = '\0';
  int len = 0;
  while(read(r_file, out, sizeof(struct usr))){
    int wlen = strlen(out->name);
    memcpy(str+len, out->name, wlen+1);
    len += wlen;
  }
  printf("%s\n", str);
  close(r_file);
  return str;
}


//does listener do anything?
struct match * handle_client_data(int s, int listener, fd_set *master, int *fdmax){
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
    if(cliSig == ACCMATCH){
      // read again to get socket2? - can i just recv another int - does recv clear the buffer?
      int socket2;
      if((nbytes = recv(s, &socket2, sizeof socket2, 0)) <= 0){ // got error or connection closed by client
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
      int sendSig = STARTMATCH;
      send(s, &sendSig, sizeof(int), 0);
      send(socket2, &sendSig, sizeof(int), 0);
      // remember to block on both ports on main
      return matchlogic(s, socket2);
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
      recv(s, unamebuff, 256, 0);       //await username and pwd input - note is blocking?
      recv(s, upwdbuff, 256, 0);
      struct usr * temp = searchDB(unamebuff, upwdbuff);
      if (!temp)
      {
        sendSig = DENY;
        send(s, &sendSig, sizeof(int), 0);
      }
      else
      {
        int a_file = open("./activeplayers.ussv", O_WRONLY|O_APPEND|O_CREAT, 0666);
        if (a_file == -1) {
          perror("open");
          return NULL;
        }
        printf("afile %d\n", a_file);
        write(a_file, temp, sizeof(struct usr));
        write(a_file, &s, sizeof(int)+1);
        close(a_file);
        sendSig = CNFRM;
        send(s, &sendSig, sizeof(int), 0);
        printf("User %s connected\n", temp->name);
        send(s, temp, sizeof(struct usr), 0);
      }
    }
    if (cliSig==REQPLYRS)
    {
      char* ret = playerList();
      send(s, ret, sizeof(ret), 0);

    }
    if (cliSig==REQMATCH){
      char opponent[256];
      recv(s, opponent, 256, 0);
      int oppsocket = searchSocket(opponent);
      printf("oppsocket:%d\n", oppsocket);
      if(oppsocket <= 0){
        int sendSig = DENY;
        send(s, &sendSig, sizeof(int), 0);
      }
      else{
        int sendSig = WAITONRESPONSE;
        send(s, &sendSig, sizeof(int), 0);
        //char *pname = searchPlayer(s)->name;

        //printf("print string %s\n", pname);
        //printf("print anything\n");
        //char out[5000];
        //sprintf(out, "Player %s would like to play you in a match", searchPlayer(s)->name);
        //out[4999] = '\0';
        //printf("%s\n", out);
        int oppSig = REQMATCH;
        send(oppsocket, &oppSig, sizeof(oppSig), 0);
        send(oppsocket, &s, sizeof(s),0);
        //send(oppsocket, searchPlayer(s)->name, sizeof(searchPlayer(s)->name), 0);
      }
    }
    if(cliSig==ACCMATCH){
      int opponent = 0;
      recv(s, &opponent, sizeof(int), 0);
      matchlogic(s, opponent);
    }
    if(cliSig==DENYMATCH){
      int opponent = 0;
      recv(s, &opponent, sizeof(int), 0);
      int oppSig = DENYMATCH;
      send(opponent, &oppSig, sizeof(oppSig), 0);
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
    else if (cliSig==REQLDBRD)
    {
      FILE* fp = fopen("./userdata.ussv", "r");
      fseek(fp, 0, SEEK_END);
      int size = ftell(fp);

      send(s, &size, sizeof(int), 0);
      send(s, getPlayers(), size, 0);
    }
  }
  return NULL; //perhaps bad practice
}


// when match terminates, it will return the score and then main server will unblock from those sockets
// on main server it just returns the information the main server will need so that it can pair up
struct * board matchlogic(int socket1, int socket2){
  int subpid = fork();
  if(subpid == 0){
    struct board * newboard = (struct board *) calloc(sizeof(board));
    newboard->pid = getpid();
    newboard->socket1 = socket1;
    newboard->socket2 = socket2;
    // index 0 of first array is personal, 1 is opp
    int board1[2][3][3] = {0};
    int board2[2][3][3] = {0};
    // on the board: 0 is blank, 1 is ur own ship, 2 is the ship got hit, -1 is someone guessed and miss
    // here add the initialization of the board and where the client want their ships to be. 3 Boats.
    for (int j = 0; j < 3; j++){
    bytes = read(socket1, buffer, sizeof(buffer));
    if (bytes == -1){
      err(server_socket, "read failed");
    }
    sscanf(buffer, "%d %d", &x, &y);
    board1[0][x][y] = 1;
  }
  for (int j = 0; j < 3; j++){
  bytes = read(socket2, buffer, sizeof(buffer));
  if (bytes == -1){
    err(server_socket, "read failed");
  }
  sscanf(buffer, "%d %d", &x, &y);
  board2[0][x][y] = 1;
}
    char buffer[256];
    while(1){ // loop of gameplay starts here
    // check for end of game here
    //board 1 check first
    int count = 0;
    for (int i = 0; i < 3; i++){
      for (int e = 0; e < 3; e++){
        if (board1[0][i][e] == 2){
          count++;
        }
      }
    }
    if (count == 3){
      printf("Board 1 won the game\n");
      exit(1);
    }
    count = 0;
    for (int i = 0; i < 3; i++){
      for (int e = 0; e < 3; e++){
        if (board2[0][i][e] == 2){
          count++;
        }
      }
    }
    if (count == 3){
      printf("Board 2 won the game\n");
      exit(1);
    }

    int bytes = read(socket1, buffer, sizeof(buffer));
    if (bytes == -1){
      err(server_socket, "read failed");
    }
    bytes = write(socket2, buffer, sizeof(buffer));
    if (bytes == -1){
      err(server_socket, "write failed");
    }

    //calculation of player1
    int h;
    int p;
    sscanf(buffer, "%d %d", &h, &p);
    if (board2[0][h][p] == 1){
      board1[1][h][p] = 2;
      board2[0][h][p] = 2;
    }
    else{
      board1[1][h][p] = -1;
      board2[0][h][p] = -1;
    }

    bytes = read(socket2, buffer, sizeof(buffer));
    if (bytes == -1){
      err(server_socket, "write failed");
    }
    bytes = write(socket1, buffer, sizeof(buffer));
    if (bytes == -1){
      err(server_socket, "write failed");
    }
    // This massive block up here is just reading the guess each client chose and send the guess to their opponent.
    // Here make the calcuation of the board and send them back to the client. Player 2 here

    sscanf(buffer, "%d %d", &h, &p);
    if (board1[0][h][p] == 1){
      board2[1][h][p] = 2;
      board1[0][h][p] = 2;
    }
    else{
      board2[1][h][p] = -1;
      board1[0][h][p] = -1;
    }


    bytes = write(socket1, board, sizeof(board));
    if (bytes == -1){
      err(server_socket, "read failed");
    }
    // Board two calc Here
    bytes = write(socket2, board, sizeof(board));
    if (bytes == -1){
      err(server_socket, "read failed");
    }
  }

  }
  else{
    return subpid;
  }
}




/*straight BEEJ's code for converting a socket into an IP address striprintfng
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
