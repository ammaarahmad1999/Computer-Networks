#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
struct data
{
	char message[255];
	int sequence;
};
void error(const char *msg)			//If error then exit
{
	perror(msg);
	exit(1);
}
int main(int argc, char *argv[])	
{
	int portno = 8080;
	int sockfd, newsockfd;		
	struct sockaddr_in serv_addr, cli_addr;		//Give internet address included in metinet
	socklen_t clilen;							//32 Bit data type
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
	{
		error("Error opening Socket.");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Binding Failed");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("Error on Accept");
	struct data buffer;
	int maxseq, n, num, ack=0;
	recv(newsockfd, &num, sizeof(num), 0);
	printf("Number of packets: %d\n",num);
	recv(newsockfd, &n, sizeof(n), 0);
	printf("Window Size is %d\n",n);
	send(newsockfd, &ack, sizeof(ack), 0);
	printf("Acknowledgement sent\n");
	maxseq = n+1;
	int recieved = 0;
	while(recieved < num)
	{
		int left = num - recieved;
		for (int i=0; i<n; i++)
		{
			if (i == left)
				break;
			bzero(&buffer, sizeof(buffer));
			recv(newsockfd, &buffer, sizeof(buffer), 0);
			if(buffer.sequence == ack)
			{
				ack = (ack+1)%maxseq;
				printf("%s\n",buffer.message);
				recieved ++;
			}
			if(recieved == num)
				break;
		}
		send(newsockfd, &ack, sizeof(ack), 0);
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}
