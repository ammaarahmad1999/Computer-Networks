#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main(int argc, char *argv[])	
{
	char *ip = "127.0.0.1";
    int portno = 8080;
	int sockfd, newsockfd, n;					//n determines success or failure
	char buffer [255];							//message
	struct sockaddr_in serv_addr, cli_addr;		//Give internet address included in metinet
	socklen_t clilen;							//32 Bit data type

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
	{
		printf("Error opening Socket.\n");
		exit(1);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Binding Failed.\n");
		exit(1);
	}
	else
		printf("Binding successfull.\n");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		printf("Error on Accept.\n");
	int length;
	n = recv(newsockfd, &length, sizeof(length), 0);
	if (n<0)
	{
		printf("Error in recieving message\n");
		exit(1);
	}
	n = recv(newsockfd, buffer, length, 0);
	if (n<0)
	{
		printf("Error in recieving message\n");
		exit(1);
	}
	char reverse[length];
	for (int i=0; i<length; i++)
	{
		printf("%c",buffer[i]);
		reverse[i] = buffer[length-i-1];
	}
	printf("\n");
	n = send(newsockfd, reverse, length, 0);
	if(n<0)
	{
		printf("Error in sending message.\n");
		exit(1);
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}
