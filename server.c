#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <time.h>

int main(int argc , char *argv[])

{
    //socket create
	int inputBuffer;
    int sockfd = 0,forClientSockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket connect
    struct sockaddr_in serverInfo,clientInfo;
    socklen_t addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(8700);
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,5);

	struct timespec t1, t2;
	int i;	
	clock_gettime(CLOCK_REALTIME, &t1);
	for(i=0;i<1000;i++){}
	clock_gettime(CLOCK_REALTIME, &t2);
	printf("Time: %d\n",t2.tv_nsec - t1.tv_nsec);
	

    while(1){
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
        recv(forClientSockfd,inputBuffer,sizeof(inputBuffer),0);
        send(forClientSockfd,message,sizeof(message),0);
        printf("Get:%s\n",inputBuffer);
    }
    return 0;
}
