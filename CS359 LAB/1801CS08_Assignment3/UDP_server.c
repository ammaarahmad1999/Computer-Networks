#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/md5.h>
#define SIZE 1032
char check[SIZE];
struct sockaddr_in server_addr, client_addr; 
struct packets
{
	int size;
	char data[1024];
	int sequence;
}packet;
struct acknowledge
{
	int sequence;
	int size;
}result;
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
    for(i = 0; i < MD5_DIGEST_LENGTH; i++) 
    {
        char temp[3];
        sprintf(temp, "%02x", c[i]);
        strcat(check,temp);
    }
    fclose (inFile);
    return 0;
}
void write_file(int sockfd)
{
	int n, len, sz, chunks;
	FILE *fp; 
	char filename[SIZE];
	char buffer[SIZE];
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	if (recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr *) &client_addr, &len)<=0)
	{
		perror("[-]Error Recieving Hello message\n");
		return;
	}
	else
		printf("CLIENT: HELLO\n");
	if(sendto(sockfd, "HELLO\0", sizeof("HELLO\0"), 0, (struct sockaddr *) &client_addr, sizeof(client_addr)) == -1)
	{
		perror("[-]Error in sending acknowledgement message.\n");
        exit(1);
	}
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));	
	if (recvfrom(sockfd, &packet, SIZE, 0 ,(struct sockaddr *) &client_addr, &len)<=0)
	{
		perror("Error recieving filename\n");
		exit(1);
	}
	else
	{
		strcpy(filename,packet.data);
		printf("Recieved filename: %s\n",filename);
		sendto(sockfd, filename, sizeof(filename), 0, (struct sockaddr *) &client_addr, sizeof(client_addr));
	}
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	n = recvfrom(sockfd, &sz, sizeof(sz), 0, (struct sockaddr *) &client_addr, &len);
	if (n<0)
	{
		perror("Error recieving file size\n");
		exit(1);
	}
	else
		printf("Recieved file size: %d Bytes\n",sz);
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	n = recvfrom(sockfd, &chunks, sizeof(chunks), 0, (struct sockaddr *) &client_addr, &len);
	if (n<0)
	{
		perror("Error recieving number of chunks\n");
		exit(1);
	}
	else
		printf("Number of chunks: %d \n",chunks);
	fp = fopen(filename, "wb");
	int i=0;
	while (i<chunks) 
	{
		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
		n = recvfrom(sockfd, &packet, SIZE, 0,  (struct sockaddr *) &client_addr, &len);
		if (n > 0 && packet.sequence == i)
		{
			result.sequence = packet.sequence+1;
			result.size = packet.size;
			sendto(sockfd, &result, sizeof(result), 0, (struct sockaddr *) &client_addr, sizeof(client_addr));
			fwrite(packet.data, 1, packet.size, fp);
			bzero(&packet, SIZE);
			i++;
		}
		else
		{
			printf("Timeout or Error occured\n");
			exit(1);
		}
	}
	fclose(fp);
	printf("[+]Data written in the file successfully.\n");
	check_sum(filename);
	sendto(sockfd, check, sizeof(check), 0,  (struct sockaddr *) &client_addr, sizeof(client_addr));
	return;
}

/*void write_file(int sockfd)
{
	int n;
	int sz;
	FILE *fp;
	char filename[SIZE];
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
}*/

int main()
{
	char *ip = "127.0.0.1";
	int port = 8080;
	int e;
	int sockfd;
	struct sockaddr_in server_addr, new_addr;
	socklen_t addr_size;
	char buffer[SIZE];
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		perror("socket creation failed");  
		exit(1);   
	}  
    memset(&server_addr, 0, sizeof(server_addr)); 
    memset(&client_addr, 0, sizeof(client_addr)); 
	printf("[+]Server socket created successfully.\n");
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);	
	server_addr.sin_addr.s_addr = INADDR_ANY;
	// Bind the socket with the server address 
    if (bind(sockfd, (struct sockaddr *)&server_addr,  sizeof(server_addr)) < 0) 
    {
    	perror("bind failed");
    	exit(1);
    }     
	printf("[+]Binding successfull.\n");
	write_file(sockfd);
	close(sockfd);
	return 0;
}
