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
	int maxseq, n, quit=0, num, expected=0;
	recv(newsockfd, &num, sizeof(num), 0);
	printf("Number of packets: %d\n",num);
	recv(newsockfd, &n, sizeof(n), 0);
	printf("Window Size of sender is %d\n",n);
	struct acknow ack;
	ack.nak = 0;
	ack.sequence = 0;
	send(newsockfd, &ack, sizeof(ack), 0);
	printf("Acknowledgement sent\n");
	maxseq = 2*n;
	struct data buffer, reciever[n];
	for (int i=0; i<n; i++)
		reciever[i].sequence=-1;
	while(num)
	{
		bzero(&buffer, sizeof(buffer));
		recv(newsockfd, &buffer, sizeof(buffer), 0);
		int index = buffer.sequence - expected;
		//printf("%d\n",index);
		if(index < 0)
			index = index + maxseq;
		if(index == 0)
		{
			printf("%s\n",buffer.message);
			expected = (expected+1)%maxseq;
			while(index<n-1)
			{
				index++;
				if(reciever[index].sequence==-1)
					break;
				printf("%s\n",reciever[index].message);
				reciever[index].sequence=-1;
				expected = (expected+1)%maxseq;
			}
			int i=0;
			while(index<n-1)
			{
				index++;
				i++;
				reciever[i]=reciever[index];
				reciever[index].sequence=-1;
			}
			ack.nak=0;
			ack.sequence = expected;
			send(newsockfd, &ack, sizeof(ack), 0);
			num--;
		}	
		else
		{
			if(index > 0 && index < n)
			{
				reciever[index]=buffer;
				num--;
			} 
			ack.nak=1;
			ack.sequence = expected;
			send(newsockfd, &ack, sizeof(ack), 0);				
		}
		
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}
