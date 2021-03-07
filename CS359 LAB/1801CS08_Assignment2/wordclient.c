#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#define PORT 8080  
  
// Driver code
void error(const char *msg)         //If error then exit
{
    perror(msg);
    exit(1);
} 
int main() 
{ 
    int sockfd; 
    char buffer[256];  
    struct sockaddr_in servaddr; 
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )  
        error("socket creation failed");  
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int n, len; 
    printf("Enter the file name: ");
    scanf("%s",buffer);
    sendto(sockfd, (const char*)buffer, strlen(buffer), 0, (const struct sockaddr *) &servaddr,  sizeof(servaddr)); 
    bzero(buffer, 256);
    n = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *) &servaddr, &len); 
    buffer[n]='\0';
    char *not = "NOTFOUND";
    if(strcmp(buffer, not))
    {
        FILE *f = fopen("newfile.txt", "w");
        sendto(sockfd, "WORD1", strlen("WORD1"), 0, (const struct sockaddr *) &servaddr,  sizeof(servaddr)); 
        bzero(buffer, 256);
        n = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *) &servaddr, &len); 
        buffer[n]='\0';
        int i=2;
        while(strcmp(buffer, "END\0"))
        {
            fprintf(f, "%s ",&buffer[0]);
            char str[10];
            snprintf(str, 10, "WORD%d", i);
            sendto(sockfd, (const char*)str, strlen(str), 0, (const struct sockaddr *) &servaddr,  sizeof(servaddr)); 
            bzero(buffer, 256);
            n = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *) &servaddr, &len);
            buffer[n]='\0';
            i++;
        }
        fclose(f);
        printf("Data Transfer complete\n");
    }   
    else
        printf("File Not Found\n");
    close(sockfd); 
    return 0; 
} 