/*
 * Send a UDP packet to server.c.
 */

#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DATA "The sea is calm tonight, the tide is full . . ."

main(int argc, char *argv[])
{
  int sock;
  struct sockaddr_in name;
  struct hostent *hp, *gethostbyname();

  if(argc != 3){
    fprintf(stderr, "Usage: client host port\n");
    exit(1);
  }

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0){
    perror("socket");
    exit(1);
  }

  hp = gethostbyname(argv[1]);
  if(hp == 0){
    fprintf(stderr, "%s: unknown host\n", argv[1]);
    exit(1);
  }
  bcopy(hp->h_addr, &name.sin_addr, hp->h_length);
  name.sin_family = AF_INET;
  name.sin_port = htons(atoi(argv[2]));
  if((int)sendto(sock, DATA, sizeof(DATA), 0, &name, sizeof(name)) < 0)
    perror("sendto");

  exit(0);
}
