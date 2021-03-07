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

	FILE *f;
	char input[255];
	bzero(input, 255);
	read(newsockfd, input, 255);
	//printf("%s\n",input);
	f = fopen(input, "r");
	if(f == NULL)
	{
		write(newsockfd, "NOTFOUND", 10);
	}
	else
	{
		while(f)
		{
			fscanf(f, "%s", buffer);
			write(newsockfd, buffer, sizeof(buffer));
			bzero(input, 255);
			read(newsockfd, input, 255);
		}
		printf("The file reading completed.\n");
		write(newsockfd, "ERROR", 5);
		fclose(f);
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}
