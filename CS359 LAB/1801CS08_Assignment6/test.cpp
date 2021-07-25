#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/md5.h>
#include <bits/stdc++.h>
#define SIZE 1048576
using namespace std;
vector <pair<int,int>> buffer;
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
vector <pair<int,int>> encoder(string s)
{
	buffer.clear();
	for (int i=0; i<s.length(); i++)
	{
		int x = s[i];
		int y = x/16;
		int z = x%16;
		cout<<x<<" "<<y<<" "<<z<<endl;
		y = encode(y);
		z = encode(z);
		cout<<x<<" "<<y<<" "<<z<<endl;
		buffer.push_back({y,z});
	}
	return buffer;
}
string decoder(vector <pair<int,int>> buffer)
{
	string s;
	for (int i=0; i<buffer.size(); i++)
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
int main()
{
	string s;
	cin>>s;
	buffer = encoder(s);
	cout<<decoder(buffer)<<endl;
}