
// Server side implementation of UDP client-server model 
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
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )  
        error("socket creation failed");  
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  sizeof(servaddr)) < 0) 
        error("bind failed"); 

    while(1)
    {
        int len, n; 
        len = sizeof(cliaddr);  //len is value/resuslt 
        n = recvfrom(sockfd, buffer, 256,  0, (struct sockaddr *) &cliaddr, &len);
        buffer[n]='\0';
        FILE *fp = fopen(buffer, "r");
        if(fp==NULL)
        {
            char *str = "NOTFOUND";
            sendto(sockfd, str, strlen(str), 0, (const struct sockaddr *) &cliaddr, len); 
        }
        else
        {
            bzero(buffer, 256);
            fscanf(fp, "%s",buffer);
            sendto(sockfd, buffer, strlen(buffer),  0, (const struct sockaddr *) &cliaddr, len); 
            while(1)
            {
                if(strcmp(buffer,"END")==0)
                    break;
                bzero(buffer, 256);
                n = recvfrom(sockfd, buffer, 256,  0, (struct sockaddr *) &cliaddr, &len);
                buffer[n]='\0';
                bzero(buffer, 256);
                fscanf(fp, "%s",buffer);
                sendto(sockfd, buffer, strlen(buffer),  0, (const struct sockaddr *) &cliaddr, len);
            }
            fclose(fp);
        }
    }
    close(sockfd); 
    return 0; 
} 
