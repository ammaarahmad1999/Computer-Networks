#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
void error(const char *msg)			//If error then exit
{
	perror(msg);
	exit(1);
}
int main(int argc, char *argv[])	
{
	if (argc < 2)
	{
		fprintf(stderr, "Port No not provided, Program terminated\n");
		exit(1);
	}
	int sockfd, newsockfd, portno, n;			//n determines success or failure
	char buffer [255];							//message
	struct sockaddr_in serv_addr, cli_addr;		//Give internet address included in metinet
	socklen_t clilen;							//32 Bit data type

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
	{
		error("Error opening Socket.");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
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

	int num1, num2, ans, choice;
S:	n = write(newsockfd, "Enter Number 1 : ", strlen("Enter Number 1"));
	if (n<0)
			error("Error writing to socket");
	read(newsockfd, &num1, sizeof(int));
	printf("Client - Number 1 is : %d\n", num1);

	n = write(newsockfd, "Enter Number 2 : ", strlen("Enter Number 2"));
	if (n<0)
			error("Error writing to socket");
	read(newsockfd, &num2, sizeof(int));
	printf("Client - Number 2 is : %d\n", num2);

	char s[] = "Enter your choice : \n1. Addition\n2. Subtraction\n3. Exit";
	n = write(newsockfd, s, strlen(s));
	read(newsockfd, &choice, sizeof(int));
	printf("Client - Choice is : %d\n", choice);

	switch(choice)
	{
		case 1:
			ans = num1+num2;
			break;
		case 2:
			ans = num1-num2;
			break;
		case 3:
			goto Q;
			break;
	}
	write(newsockfd, &ans, sizeof(int));
	if(choice != 3)
		goto S;

Q:	close(newsockfd);
	close(sockfd);
	return 0;
}
