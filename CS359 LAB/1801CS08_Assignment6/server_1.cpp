#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <bits/stdc++.h>
#define SIZE 1048576
using namespace std;
vector <pair<int,int>>buffer;
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
vector<pair<int,int>> encoder(string s)
{
	buffer.clear();
	for (int i=0; i<s.length(); i++)
	{
		int x = s[i];
		int y = x/16;
		int z = x%16;
		y = encode(y);
		z = encode(z);
		buffer.push_back({y,z});
	}
	return buffer;
}
string decoder(vector<pair<int,int>> buffer)
{
	string s;
	for (int i=0; i<buffer.size(); i+=2)
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
int main(int argc, char *argv[])	
{
	string ip = "127.0.0.1";
    int portno = 8080;
	int sockfd, newsockfd, n;					//n determines success or failure
	struct sockaddr_in serv_addr, cli_addr;		//Give internet address included in metinet
	socklen_t clilen;							//32 Bit data type

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd<0)
	{
		printf("Error opening Socket.\n");
		exit(1);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Binding Failed.\n");
		exit(1);
	}
	else
		printf("Binding successfull.\n");
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		printf("Error on Accept.\n");
	string message;
	while(1)
	{
		char sms[255];
		n = recv(newsockfd, sms, SIZE, 0);
		if (n<0)
		{
			printf("Error in recieving message\n");
			exit(1);
		}
		//message = decoder(buffer);
		cout<<"CLIENT: "<<message<<endl;
		getline(cin, message);
		n = send(newsockfd, &message, message.length(), 0);
		if (n<0)
		{
			printf("Error in sending message\n");
			exit(1);
		}

	}

	close(newsockfd);
	close(sockfd);
	return 0;
}
