#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>

double time_diff(struct timespec t1, struct timespec t2)
{
    return (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_nsec - t1.tv_nsec)/1000000000;
}

int main(int argc , char *argv[])
{

    //socket create
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0); 

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }   

    //socket connect
    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    char addr[30];
	printf("IP:");
	scanf("%s", addr);

    //localhost test
    info.sin_addr.s_addr = inet_addr(addr);
    info.sin_port = htons(8700);

	printf("Connecting...\n");
    int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
    if(err==-1){
        printf("Connection error\n");
    }
	
        
    //Send a message to server
    int sendBuffer;
    double recvBuffer;
    int testCount;
    
    printf("Loop count:");
    scanf("%d",&sendBuffer);
    printf("Testcase count:");
    scanf("%d",&testCount);

    int i;

    struct timespec t1, t2;
    double total_time;

    send(sockfd, &testCount,sizeof(testCount),0);
    
    printf("Total\t\tUE2MEC\t\tMEC\t\tMEC2Cloud\tCloud\n");
    for(i=0;i<testCount;i++){
    	clock_gettime(CLOCK_REALTIME, &t1);
        send(sockfd, &sendBuffer,sizeof(sendBuffer),0);
        recv(sockfd, &recvBuffer,sizeof(recvBuffer),0);
    	clock_gettime(CLOCK_REALTIME, &t2);
        total_time = time_diff(t1,t2);
	//printf("Time: %lf\n",(double)recvBuffer/10000000);
        printf("%lf\t%lf\t%lf\n",total_time,total_time-recvBuffer,recvBuffer);
    }
    printf("Close Socket.\n");
    close(sockfd);
    return 0;
}
