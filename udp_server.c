/*
 * Receive a UDP packet from client.c.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

main(int argc, char *argv[])
{
  int sock, fromlen;
  short port;
  struct sockaddr_in name, from;
  char buf[1024];

  if(argc != 2){
    fprintf(stderr, "Usage: server port\n");
    exit(1);
  }
  port = atoi(argv[1]);

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0){
    perror("socket");
    exit(1);
  }

  name.sin_family = AF_INET;
  name.sin_addr.s_addr = INADDR_ANY;
  name.sin_port = ntohs(port);
  if(bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0){
    perror("bind");
    exit(1);
  }
  
  fromlen = sizeof(from);
  if(recvfrom(sock, buf, sizeof(buf), 0,
	      (struct sockaddr *)&from, &fromlen) < 0)
    perror("recvfrom");
  printf("from %s: %s\n", inet_ntoa(from.sin_addr), buf);
  close(sock);

  exit(0);
}

