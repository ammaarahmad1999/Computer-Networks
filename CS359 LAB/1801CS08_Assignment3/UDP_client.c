/*The client performs the following functions.

    Start the program
    Declare the variables and structures required.
    A socket is created and the connect function is executed.
    The file is opened.
    The data from the file is read and sent to the server.
    The socket is closed.
    The program is stopped.
*/

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
char checksum[SIZE];
char newfile[256];
struct sockaddr_in server_addr;
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
    //printf("%s\n",check);
    fclose (inFile);
    return 0;
}
void send_file(FILE *fp, int sockfd)
{
    int n; int len;
    char data[SIZE] = {0};
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    int SZ = 2*sizeof(int);
    rewind(fp);
    struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
    char *hello="HELLO\0";
    if (sendto(sockfd, hello, strlen(hello), 0, (const struct sockaddr *) &server_addr,  sizeof(server_addr)) == -1) 
    {
        perror("[-]Error in sending HELLO message.\n");
        exit(1);
    }
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (recvfrom(sockfd, data, SIZE, 0, (struct sockaddr *) &server_addr, &len) <=0)
    {
        perror("No acknoledgement from server.\n");
        exit(1);
    }
    else
        printf("SERVER: HELLO\n");
    bzero(data, SIZE);
    strcpy(packet.data, newfile);
    packet.size = strlen(packet.data);
    packet.sequence = 0;
    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *) &server_addr,  sizeof(server_addr))  == -1)
    {
        perror("[-]Error in sending filename.\n");
        exit(1);
    }
    else
    {
    	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        if(recvfrom(sockfd, data, SIZE, 0, (struct sockaddr *) &server_addr, &len)<0)
        {
        	printf("Timeout Error\n");
        	exit(1);
        }
    }
    if(strcmp(data, newfile))
    {
    	printf("%s %s",data,newfile);
        printf("Filename chamged\n");
        exit(1);
    }
    bzero(&packet, SIZE);
    bzero(data, SIZE);
    if (sendto(sockfd, &sz, sizeof(int), 0, (struct sockaddr *) &server_addr,  sizeof(server_addr))  == -1)
    {
        perror("[-]Error in sending file size.\n");
        exit(1);
    }
    int chunks = (sz+SIZE-SZ-2)/(SIZE-SZ-1);
    if (sendto(sockfd, &chunks, sizeof(int), 0, (struct sockaddr *) &server_addr,  sizeof(server_addr))  == -1)
    {
        perror("[-]Error in sending number of chunks.\n");
        exit(1);
    }
    char c;
    int i=0, ack=1;
    while(i<chunks) 
    {
    	if(ack == 1)
    	{
    		n=fread(packet.data, 1, SIZE-SZ-1, fp);
        	packet.sequence = i;
        	packet.data[1023]='\0';
        	packet.size = strlen(packet.data);
    	}
        sendto(sockfd, &packet, SIZE, 0, (struct sockaddr *) &server_addr,  sizeof(server_addr));
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        int res = recvfrom(sockfd, &result, SZ, 0, (struct sockaddr *) &server_addr, &len);
        if(res>0 && result.sequence == i+1 && result.size == packet.size)
        {
            bzero(&packet, sizeof(packet));
        	i++;
        }
        else
        {
        	ack=0;
        	printf("Acknowledgement Not recieved for packet No. %d\n",i);
        	printf("Timeout reached. Resending Packet No. .%d\n",i);
        }
    }
    recvfrom(sockfd, checksum, SIZE, 0, (struct sockaddr *) &server_addr,  &len);
    return;
}


int main()
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;
    int sockfd;
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )  
    {
        perror("socket creation failed");  
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");
    memset(&server_addr, 0, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    FILE *fp;
    char filename[256];
    printf("Enter the filename\n");
    scanf("%s",filename);
    strcat(filename, "\0");
    fp = fopen(filename, "rb");
    if (fp == NULL) 
    {
        perror("[-]Error in reading file.\n");
        exit(1);
    }
    printf("Enter the filename in which data needs to be copied\n");
    scanf("%s",newfile);
    strcat(newfile, "\0");
    send_file(fp, sockfd);
    check_sum(filename);
    if(strcmp(checksum, check)==0)
    {
        printf("[+]File data sent successfully.\n");
        printf("Both file has equal checksum: %s\n",check);
    }   
    else
        printf("[-]File data got corrupted\n");
    printf("[+]Closing the connection.\n");
    close(sockfd);
    return 0;
}
