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

	while(1)
	{
		bzero (buffer, 255);					//Clearing buffer
		n = read(newsockfd, buffer, 255);
		if (n<0)
			error ("Error on reading.");
		printf("Client : %s\n", buffer);
		fgets(buffer, 255, stdin);
		n = write(newsockfd, buffer, strlen(buffer));

		if (n<0)
			error ("Error on writing");

		int i = strncmp("Bye", buffer, 3);
		if (i==0)
			break;

	}

	close(newsockfd);
	close(sockfd);
	return 0;
}
