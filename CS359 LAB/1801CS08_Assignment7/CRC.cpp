#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>					
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/md5.h>
#include <netdb.h>
#include <bits/stdc++.h>
#define SIZE 8192
using namespace std;
string divide;
string binary(int x)
{
	cout<<x<<endl;
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
bool error(string rem)
{
	for (int i=0; i<rem.length(); i++)
		if(rem[i]=='1')
			return true;
	return false;
}
int main()
{
	string s;
	cout<<"Enter the binary CRC checker"<<endl;
    cin>>divide;
    cout<<"Enter the binary string"<<endl;
	cin>>s;
	string code = encoder(s);
	code = s+code;
	cout<<"Encoded string: "<<code<<endl;
	cout<<"Enter the bits to be flipped"<<endl;
	int x;
	cin>>x;
	if(x<code.length())
		code[x]=code[x]^1;
	string rem = crc(code);
	int ch = error(rem);
	if(ch == 0)
		cout<<"No error: Remainder = "<<rem<<endl;
	else
		cout<<"Error: Remainder = "<<rem<<endl;
}