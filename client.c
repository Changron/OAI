#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>

struct OAI_time {
	double total;
	double ue2mec;
	double mec;
	double mec2cloud;
	double cloud;
};

double time_diff(struct timespec t1, struct timespec t2)
{
    return (t2.tv_sec - t1.tv_sec) + (double)(t2.tv_nsec - t1.tv_nsec)/1000000000;
}

int main(int argc , char *argv[])
{
	int i;

	// read iplist
    char addr[5][30];
	char location[5][30];
	int ip_count = 0;
	FILE *iplist;
	iplist = fopen("iplist","r");
	if(iplist){
		fscanf(iplist,"%d",&ip_count);
		for(i=0;i<ip_count;i++){
			fscanf(iplist,"%s %s",location[i],addr[i]);
			printf("Instances %d: %s\t%s\n", i+1,location[i],addr[i]);
		}
	}
	else printf("IP list file not found.");

	// read testcase from stdin
	int loopCount, testCount;	
	printf("Loop count:");
	scanf("%d",&loopCount);
	printf("Testcase count:");
	scanf("%d",&testCount);

	// statistic
	struct OAI_time inst_stat[ip_count];

	// connect to servers and perform calculation	
	int instance = 0;
	for(instance = 0;instance<ip_count;instance++){	
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

			// connect to server
			info.sin_addr.s_addr = inet_addr(addr[instance]);
			info.sin_port = htons(8700);

			printf("Connecting %s...\n",location[instance]);
			int err = connect(sockfd,(struct sockaddr *)&info,sizeof(info));
			if(err==-1){
				printf("Connection error.\n");
				continue;
			}
			printf("Connection to %s succeed.\n",location[instance]);			
	
			//Send a message to server
			int sendBuffer;
			double recvBuffer;
			
			sendBuffer = loopCount;	

			struct timespec t1, t2;
			double total_time;

			struct OAI_time inst_time;
			inst_time.total = 0;
			inst_time.ue2mec = 0;
			inst_time.mec = 0;
			inst_time.mec2cloud = 0;
			inst_time.cloud = 0;

			send(sockfd, &testCount,sizeof(testCount),0);
			
			printf("Total\t\tUE2MEC\t\tMEC\t\tMEC2Cloud\tCloud\n");
			for(i=0;i<testCount;i++){
				clock_gettime(CLOCK_REALTIME, &t1);
				send(sockfd, &sendBuffer,sizeof(sendBuffer),0);
				recv(sockfd, &recvBuffer,sizeof(recvBuffer),0);
				clock_gettime(CLOCK_REALTIME, &t2);
				
				total_time = time_diff(t1,t2);
				inst_time.total += total_time;
				inst_time.ue2mec += total_time-recvBuffer;
				inst_time.mec += recvBuffer;
				printf("%lf\t%lf\t%lf\n",total_time,total_time-recvBuffer,recvBuffer);
			}
			inst_time.total /= testCount;
			inst_time.ue2mec /= testCount;
			inst_time.mec /= testCount;
			inst_time.mec2cloud /= testCount;
			inst_time.cloud /= testCount;
			inst_stat[instance].total = inst_time.total;
			inst_stat[instance].ue2mec = inst_time.ue2mec;
			inst_stat[instance].mec = inst_time.mec;
			inst_stat[instance].mec2cloud = inst_time.mec2cloud;
			inst_stat[instance].cloud = inst_time.cloud;
			printf("Close Socket %s.\n",location[instance]);
			close(sockfd);
	}
	printf("Average Time Consumption:\n");
	printf("Loc\tTotal\t\tUE2MEC\t\tMEC\t\tMEC2Cloud\tCloud\n");
	for(i=0;i<ip_count;i++){
		printf("%s\t%lf\t%lf\t%lf\t%lf\t%lf\n",location[i],inst_stat[i].total,inst_stat[i].ue2mec,
				inst_stat[i].mec,inst_stat[i].mec2cloud,inst_stat[i].cloud);
	}
    return 0;
}
