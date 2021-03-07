/*
filename server_ipaddress portno
argv[0] filename
argv[1] server_ipaddress
argv[2] portno

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

struct data
{
	char message[256];
	int sequence;
};
struct acknow
{
	int nak;
	int sequence;
};
void error(const char *msg)			//If error then exit
{
	perror(msg);
	exit(1);
}
int main(int argc, char *argv[])	
{
	char *ip = "127.0.0.1";
    int portno = 8080;
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
		error("Error opening socket");
	server = gethostbyname(ip);
	if (server == NULL)
	{
		fprintf(stderr, "Error, no such host");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
		error("Connection failed");
	struct data buffer;
	struct acknow ack;
	int maxseq, n, quit=0, num, sent=0;
	printf("Enter the total number of packets to be sent\n");
	scanf("%d",&num);
	send(sockfd, &num, sizeof(num), 0);
	printf("Enter the window size of buffer\n");
	scanf("%d",&n);
	maxseq = 2*n; char c;
	printf("Note: Valid Sequence Numbers are from 0 to %d\n",maxseq-1);
	send(sockfd, &n, sizeof(n), 0);
	recv(sockfd, &ack, sizeof(ack), 0);
	printf("Enter the sequence number followed by message to be sent for every packet\n");
	printf("Acknowledgement recieved to send package with sequence No. %d\n",ack.sequence);
	int expected = 0;
	while(expected<n)
	{
		bzero(&buffer, sizeof(buffer));
		scanf("%d",&buffer.sequence);
		scanf("%c %255[^\n]",&c,buffer.message);
		while(buffer.sequence != expected)
		{
			printf("Incorrect Sequence Number. Please enter the sequence followed by message again\n");
			bzero(&buffer, sizeof(buffer));
			scanf("%d",&buffer.sequence);
			scanf("%c %255[^\n]",&c,buffer.message);
		}
		send(sockfd, &buffer, sizeof(buffer), 0);
		expected++;
		num--;
		if(num==0)
			break;
	}
	while(num)
	{
		recv(sockfd, &ack, sizeof(ack), 0);
		if(ack.nak==1)
			printf("Negative Acknowledgement for sequence No. %d. Resend it\n",ack.sequence);
		else
		{
			int recieved = (ack.sequence+maxseq-1)%maxseq;
			printf("Acknowledgement recieved: Frame %d recieved succesfully\n",recieved);
			num--;
		}
		bzero(&buffer, sizeof(buffer));
		scanf("%d",&buffer.sequence);
		scanf("%c %255[^\n]",&c,buffer.message);
		while(buffer.sequence != expected)
		{
			printf("Incorrect Sequence Number. Please enter the sequence followed by message again\n");
			bzero(&buffer, sizeof(buffer));
			scanf("%d",&buffer.sequence);
			scanf("%c %255[^\n]",&c,buffer.message);
		}
		expected = (expected+1)%maxseq;
		send(sockfd, &buffer, sizeof(buffer), 0);
	}
	while(n)
	{
		recv(sockfd, &ack, sizeof(ack), 0);
		if(ack.nak==1)
		{
			printf("Negative Acknowledgement for sequence No. %d. Resend it\n",ack.sequence);
			bzero(&buffer, sizeof(buffer));
			scanf("%d",&buffer.sequence);
			scanf("%c %255[^\n]",&c,buffer.message);
			send(sockfd, &buffer, sizeof(buffer), 0);
		}
		else
		{
			int recieved = (ack.sequence+maxseq-1)%maxseq;
			printf("Acknowledgement recieved: Frame %d recieved succesfully\n",recieved);
			n--;
		}
	}
	close(sockfd);
	return 0;
}