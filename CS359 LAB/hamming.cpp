#include<iostream>
#include<vector>
#include<algorithm>
#include <sstream> 
#include<cmath>
using namespace std;
int check(string s)
{
	if(s.length()!=8)
		return 0;
	reverse(s.begin(), s.end());
	int A = (s[1]+s[3]+s[5]+s[7])%2;
	int B = (s[2]+s[3]+s[6]+s[7])%2;
	int C = (s[4]+s[5]+s[6]+s[7])%2;
	int error = C*4+B*2+A;
	return error;
}
string decode(string s)
{
	int pos=check(s);
	reverse(s.begin(), s.end());
	if(pos)
		if(s[pos]=='1')
			s[pos]=='0';
		else
			s[pos]=='1';
	string code;
	code+=s[7];
	code+=s[6];
	code+=s[5];
	code+=s[3];
	return code;
}
string encode(string s)
{
	string encode;
	char p;
	encode+=s[0];
	encode+=s[1];
	encode+=s[2];
	int p4=(s[0]+s[1]+s[2])%2;
	p=p4+48;
	encode+=p;
	encode+=s[3];
	int p2=(s[0]+s[1]+s[3])%2;
	p=p2+48;
	encode+=p;
	int p1=(s[0]+s[2]+s[3])%2;
	p=p1+48;
	encode+=p;
	encode+='0';
	return encode;
}
int main()
{
	string s;
	cout<<"Enter the 4 bit binary code"<<endl;
	cin>>s;
	string ens = encode(s);
	cout<<ens.substr(0,7)<<endl;
	cout<<"Enter the bit to flip from 0 to 7"<<endl;
	int b;
	cin>>b;
	ens[7-b]=(ens[7-b]+1)%2+48;
	cout<<ens.substr(0,7)<<endl;
	int error = check(ens);
	if(error)
	{
		cout<<"There is a error at bit "<<error<<endl;
		ens[7-b]=(ens[7-b]+1)%2+48;
	}	
	string des = decode(ens);
	cout<<des<<endl;
}