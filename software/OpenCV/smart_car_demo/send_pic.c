#include<netdb.h>
#include<errno.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<malloc.h>
#include<memory.h>


extern int Socket_Send(int sockfd,unsigned char *buffer,int size)
{

	if(send(sockfd,buffer,size,0)<0)
	{
		perror("send");
	  	exit(1);
	}

}
extern int Socket_Init(char *destip,char * destport)
{

	int port = atoi(destport);
	
	int sockfd = socket(AF_INET,SOCK_STREAM,0);

	

	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	inet_pton(AF_INET,destip,&saddr.sin_addr.s_addr);

	if(connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr))<0)
	  {
			perror("connect");
			printf("connect error! stop send!\n");
			exit(1);
	  }


	return sockfd;
}
