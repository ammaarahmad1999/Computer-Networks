
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
int main(int argc, char *argv[])	
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char *ip = "127.0.0.1";
    portno = 8080;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
	{
		printf("Error opening socket.\n");
		exit(1);
	}
	server = gethostbyname(ip);
	if (server == NULL)
	{
		printf("Error, no such host.\n");
		exit(1);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	{
		printf("Connection failed.\n");
		exit(1);
	}
	else
		printf("Connected to Server.\n");
	char *buffer = "Indian Institute of Technology Patna\0";
	n = send(sockfd, buffer, strlen(buffer), 0);
	if(n<0)
	{
		printf("Error in sending message.\n");
		exit(1);
	}
	else
		printf("Message successfully sent.\n");
	close(sockfd);
	return 0;
}
