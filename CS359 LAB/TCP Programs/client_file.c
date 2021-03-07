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
void error(const char *msg)			//If error then exit
{
	perror(msg);
	exit(1);
}
int main(int argc, char *argv[])	
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[255];
	if (argc < 3)
	{
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
		error("Error opening socket");

	server = gethostbyname(argv[1]);
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
	
	bzero(buffer, 255);
	char filename[]="file.txt"; 
	int words = 0;
	write(sockfd, filename, sizeof(filename));
	read(sockfd, buffer, sizeof(buffer));
	if(strcmp("NOTFOUND", buffer)==0)
	{
		printf("File Not Found\n");
	}
	else
	{
		FILE *f;
		f = fopen("newfile.txt", "w");
		while (strcmp(buffer, "END") && strcmp(buffer, "ERROR"))
		{
			words++;
			fprintf(f, "%s ", buffer);
			char message[255];
			sprintf(message, "WORD%d", words);
			write(sockfd, message, sizeof(message));
			bzero(buffer, 255);
			read(sockfd, buffer, sizeof(buffer));
		}
		fprintf(f, "%s ", buffer);	
		fclose(f);
		printf("data copied from %s to newfile.txt\n",filename);
	}
	close(sockfd);
	return 0;
}
