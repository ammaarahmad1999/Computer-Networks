#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/md5.h>
#include <bits/stdc++.h>
#define SIZE 8192
using namespace std;
pair<int,int> buffer[1024];
void error(int bits[7])
{
	int A = bits[0]^bits[2]^bits[4]^bits[6];
	int B = bits[1]^bits[2]^bits[5]^bits[6];
	int C = bits[3]^bits[4]^bits[5]^bits[6];
	int error = C*4+B*2+A;
	if(error)
		bits[error-1] = bits[error-1]^1;
}
int decode(int x)
{
	int bits[7];
	for (int i=0; i<7; i++)
	{
		bits[i]=x%2;
		x=x/2;
	}
	error(bits);
	int z = bits[6]*8+bits[5]*4+bits[4]*2+bits[2];
	return z;
}
int encode(int x)
{
	int bits[4];
	for (int i=0; i<4; i++)
	{
		bits[i]=x%2;
		x=x/2;
	}
	int z = bits[3]*4+bits[2]*2+bits[1];
	z = z*4 + (bits[3]^bits[2]^bits[1])*2 + bits[0];
	z = z*2 + (bits[3]^bits[2]^bits[0]);
	z = z*2 + (bits[3]^bits[1]^bits[0]);
	return z;
}
void encoder(string s)
{
	for (int i=0; i<s.length(); i++)
	{
		int x = s[i];
		int y = x/16;
		int z = x%16;
		y = encode(y);
		z = encode(z);
		buffer[i]={y,z};
	}
}
string decoder(int len)
{
	string s;
	for (int i=0; i<len; i++)
	{
		int x = buffer[i].first;
		int y = buffer[i].second;
		x = decode(x);
		y = decode(y);
		int z = x*16+y;
		s.push_back(char(z));
	}
	return s;
}
void connection(int sockfd)
{
	string s; int n; int length;
	while (1) 
	{
		bzero(&buffer, sizeof(buffer));
        n = recv(sockfd, &length, sizeof(length), 0);
		if (n <= 0)
		{
			printf("[-]Error in recieving message\n");
			break;
		}
		n = recv(sockfd, &buffer, sizeof(buffer), 0);
		if (n <= 0)
		{
			printf("[-]Error in recieving message\n");
			break;
		}
		else
		{
			s = decoder(length);
			cout<<"CLIENT: "<<s<<endl;
		}
		getline(cin, s);
		bzero(&buffer, sizeof(buffer));
        length = s.length();
        if (send(sockfd, &length, sizeof(length), 0) == -1) 
        {
            printf("[-]Error in sending message.\n");
            exit(1);
        }
		encoder(s);
		if (send(sockfd, &buffer, sizeof(buffer), 0) == -1)
		{
			printf("[-]Error in sending message.\n");
        	exit(1);
		}
	}
	return;
}


int main()
{
	string ip = "127.0.0.1";
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
		perror("[-]Error in bind\n");
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
	connection(new_sock);
	close(sockfd);
	return 0;
}
