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
string divide;
string binary(int x)
{
	string ans;
	for (int i=0; i<8; i++)
	{
		ans = to_string(x%2) + ans;
		x = x/2;
	}
	return ans;
}
string crc(string code)
{
	int length = code.length();
	int len = divide.length();
	int pick = len;
	string dividend = code.substr(0,pick);
	while (pick < length)
	{
		if (dividend[0]=='1')
		{
			for (int i=0; i<len-1; i++)
				dividend[i]=(dividend[i+1]^divide[i+1])+48;
			dividend[len-1] = code[pick];
		}
		else
		{
			for (int i=0; i<len-1; i++)
				dividend[i]=dividend[i+1];
			dividend[len-1] = code[pick];
		}
		pick++;
	}
	if (dividend[0]=='1')
	{
		for (int i=0; i<len-1; i++)
			dividend[i]=(dividend[i+1]^divide[i+1])+48;
	}
	else
	{
		for (int i=0; i<len-1; i++)
			dividend[i]=dividend[i+1];
	}
	dividend.pop_back();
	return dividend;
}
string encoder(string code)
{
	int len = divide.length();
	for (int i=1; i<len; i++)
		code.push_back('0');
	string dividend = crc(code);
	return dividend;
}
int decimal(string s)
{
	int x=0;
	for (int i=0; i<s.length(); i++)
		x = x*2+(s[i]-48);
	return x;
}
string binary_to_char(string s)
{
	int len = s.length();
	string message;
	for (int i=0; i<len; i+=8)
		message += char(decimal(s.substr(i,8)));
	return message;
}
string ascii_to_binary(string s)
{
	int len = s.length();
	string code;
	for (int i=0; i<len; i++)
	{
		string bin = binary(s[i]);
		code = code + bin;
	}
	return code;
}
bool isbinary(string s)
{
	for (int i=0; i<s.length(); i++)
		if(s[i]!='0' && s[i]!='1')
			return false;
	return true;
}
bool error(string rem)
{
	for (int i=0; i<rem.length(); i++)
		if(rem[i]=='1')
			return true;
	return false;
}
void connection(int sockfd)
{
	string s, rem, sms; 
	int n;
	char buffer[1024];
	n = recv(sockfd, buffer, SIZE, 0);
	if (n <= 0)
	{
		printf("[-]Error in recieving message\n");
		exit(1);
	}
	else
	{
		divide = buffer;
		cout<<"CRC checker is: "<<divide<<endl;
	}
	int len = divide.length();
	//printf("Input should be strictly in binary format\n");
	printf("In order to exit: 0\n");
	while (1) 
	{
		
		bzero(buffer, sizeof(buffer));
		n = recv(sockfd, buffer, SIZE, 0);
		if (n <= 0)
		{
			printf("[-]Error in recieving message\n");
			break;
		}
		else
		{
			sms = buffer;
			cout<<"CRC recieved: "<<sms<<endl;
			rem = crc(sms);
			if(error(rem))
				cout<<"Error in code recieved\n";
			else
			{
				int l = sms.length();
				s = sms.substr(0,l-len+1);
				s = binary_to_char(s);
				cout<<"CLIENT: "<<s<<endl;
				if(s=="0")
					break;
			}
		}
		getline(cin, s);
        /*while(isbinary(s)==false)
        {
        	cout<<"Input is not in binary format. Please Enter again"<<endl;
        	cin>>s;
        }*/
		bzero(buffer, sizeof(buffer));
		s = ascii_to_binary(s);
        rem = encoder(s);
        sms = s + rem;
        cout<<"CRC sending: "<<sms<<endl;
        strcpy(buffer, sms.c_str());
		if (send(sockfd, &buffer, sizeof(buffer), 0) == -1)
		{
			printf("[-]Error in sending message.\n");
        	exit(1);
		}
		if(s=="00110000")
			break;
	}
	return;
}
int main()
{
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
