#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/md5.h>
#define SIZE 1024

int check_sum(char *filename)
{
    unsigned char c[MD5_DIGEST_LENGTH];
    int i;
    FILE *inFile = fopen (filename, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];
    if (inFile == NULL) 
    {
        printf ("%s can't be opened.\n", filename);
        return 0;
    }
    MD5_Init (&mdContext);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
        MD5_Update (&mdContext, data, bytes);
    MD5_Final (c,&mdContext);
    printf ("%s\n checksum:", filename);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) 
    	printf("%02x", c[i]);
    printf("\n");
    fclose (inFile);
    return 0;
}
void write_file(int sockfd)
{
	int n;
	int sz;
	FILE *fp;
	char *filename = "newfile.txt";
	char buffer[SIZE];
	if (!read(sockfd, buffer, SIZE))
		return;
	else
		printf("HELLO\n");
	if(!write(sockfd, "HELLO", sizeof("HELLO")))
	{
		perror("[-]Error in sending acknowledgement message.\n");
        	exit(1);
	}
	if (read(sockfd, &sz, sizeof(int)))
		printf("Recieved file size: %d\n", sz);
	else
		return;
	if (read(sockfd, filename, SIZE))
		printf("Recieved filename: %s\n",filename);
	else
		return;
	int num=0;
	fp = fopen(filename, "w");
	while (1) 
	{
		n = read(sockfd, buffer, SIZE);
		if (n <= 0)
			break;
		num+=n;
		//printf("%d %d\n",n,num);
		fprintf(fp, "%s", buffer);
		if(num>=sz)
		{
			fclose(fp);
			//printf("File closed\n");
			break;
		}
		bzero(buffer, SIZE);
	}
	printf("Number of bytes recieved: %d\n",num);
	check_sum(filename);
	return;
}

int main()
{
	char *ip = "127.0.0.1";
	int port = 8080;
	int e;
	int sockfd, new_sock;
	struct sockaddr_in server_addr, new_addr;
	socklen_t addr_size;
	char buffer[SIZE];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) 
	{
		perror("[-]Error in socket");
		exit(1);
	}
	printf("[+]Server socket created successfully.\n");
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);	
	server_addr.sin_addr.s_addr = INADDR_ANY;
	e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(e < 0) 
	{
		perror("[-]Error in bind");
		exit(1);
	} 
	printf("[+]Binding successfull.\n");
	if(listen(sockfd, 10) == 0)
		printf("[+]Listening....\n");
	else
	{
		perror("[-]Error in listening\n");
		exit(1);
 	}
	addr_size = sizeof(new_addr);
	new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
	write_file(new_sock);
	printf("[+]Data written in the file successfully.\n");
	close(sockfd);
	return 0;
}
