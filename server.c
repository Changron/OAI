#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <time.h>

double time_diff(struct timespec t1, struct timespec t2)
{
    return (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_nsec - t1.tv_nsec)/1000000000;

}

double work(int nstep){
    struct timespec t1, t2;
    int i;
    clock_gettime(CLOCK_REALTIME, &t1);
    for(i=0;i<nstep;i++){}
    clock_gettime(CLOCK_REALTIME, &t2);
    //return t2.tv_nsec - t1.tv_nsec;
    //printf("T1: %d %d T2: %ld %ld\n",(int)t1.tv_sec,(int)t1.tv_nsec,(long)t2.tv_sec,(long)t2.tv_nsec);
    return time_diff(t1, t2);
}

int main(int argc , char *argv[])
{
    //socket create
    int recvBuffer;
    double sendBuffer;
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

    printf("Waiting for connection...\n");
    while(1){
        forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
	printf("Connected.\n");

        int i, testCount;
        recv(forClientSockfd,&testCount,sizeof(testCount),0);
        // recv
        for(i=0;i<testCount;i++){
            recv(forClientSockfd,&recvBuffer,sizeof(recvBuffer),0);

            sendBuffer = work(recvBuffer);

            send(forClientSockfd,&sendBuffer,sizeof(sendBuffer),0);
            printf("Get:%d\n",recvBuffer);
        }
        printf("Done.\n");
    }
    return 0;
}

