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
#define SIZE 1024
char check[SIZE];
char checksum[SIZE];
char newfile[256];
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
        char *temp;
        sprintf(temp, "%02x", c[i]);
        strcat(check,temp);
    }
    //printf("%s\n",check);
    fclose (inFile);
    return 0;
}
void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    rewind(fp);
    if (send(sockfd, "HELLO", sizeof("HELLO"), 0) == -1) 
    {
        perror("[-]Error in sending HELLO message.\n");
        exit(1);
    }
    if (recv(sockfd, data, SIZE, 0) <=0)
    {
        perror("No acknoledgement from server.\n");
        exit(1);
    }
    else
        printf("SERVER: HELLO\n");
    bzero(data, SIZE);
    if (send(sockfd, newfile, sizeof(newfile), 0) == -1)
    {
        perror("[-]Error in sending filename.\n");
        exit(1);
    }
    else
        recv(sockfd, data, SIZE, 0);
    if(strcmp(data, newfile))
    {
        printf("Filename chamged\n");
        exit(1);
    }
    bzero(data, SIZE);
    //printf("%d\n",sz);
    if (send(sockfd, &sz, sizeof(sz), 0) == -1)
    {
        perror("[-]Error in sending file size.\n");
        exit(1);
    }
    char c;
    while(ftell(fp)<sz) 
    {
        fread(data, SIZE, 1, fp);
        if (send(sockfd, data, sizeof(data), 0) == -1) 
        {
            perror("[-]Error in sending file.\n");
            exit(1);
        }
        bzero(data, SIZE);
    }
    recv(sockfd, checksum, SIZE, 0);
    return;
}

/*void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};
    char *newfile = "newfile.txt";
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    rewind(fp);
    if (!write(sockfd, "HELLO", sizeof("HELLO"))) 
    {
        perror("[-]Error in sending HELLO message.\n");
        exit(1);
    }
    if (!read(sockfd, data, SIZE))
    {
        perror("No acknoledgement from server.\n");
        exit(1);
    }
    else
    	printf("SERVER: HELLO\n");
    bzero(data, SIZE);
    if (write(sockfd, &sz, sizeof(int)))
    	printf("file size sent\n");
    else
    {
        perror("[-]Error in sending file size.\n");
        exit(1);
    }	
    if (write(sockfd, newfile, sizeof(newfile)))
    	printf("file name sent\n");
    else
    {
        perror("[-]Error in sending filename.\n");
        exit(1);
    }
    
    while(fgets(data, SIZE, fp) != NULL) 
    {
        if (!write(sockfd, data, sizeof(data))) 
        {
            perror("[-]Error in sending file.\n");
            exit(1);
        }
        bzero(data, SIZE);
    }
    printf("Number of bytes in a file: %d\n",sz);
    return;
}*/

int main()
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;
    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;
    char filename[256];
    printf("Enter the filename\n");
    scanf("%s",filename);
    fp = fopen(filename, "r");
    if (fp == NULL) 
    {
        perror("[-]Error in reading file.\n");
        exit(1);
    }
    printf("Enter the filename in which data needs to be copied\n");
    scanf("%s",newfile);
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
    e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e == -1) 
    {
        perror("[-]Error in socket\n");
        exit(1);
    }
    printf("[+]Connected to Server.\n");
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
