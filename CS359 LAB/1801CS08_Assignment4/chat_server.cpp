#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/time.h> 
#include <errno.h> 
#include <sys/select.h> 
#include <bits/stdc++.h>
using namespace std;
#define MAX_BUFFER_SIZE 1024
# define BUFSIZE 1024

void error(string str) 
{
	cout<<"Error: "<<str<<endl;
	exit(1);
}
pair <string, string> getPersonAndMessage(string buffer) 
{
	int len = buffer.length();
	bool done = false;
	string person = "";
	string message = "";
	size_t found=buffer.find('/');
	if(found!=string::npos)
	{
		person = buffer.substr(0,found);
		message = buffer.substr(found+1);
	}
	return make_pair(person, message);
}
int main(int argc, char ** argv) 
{
	map <string, pair<string,int>> M;
    ifstream fp;
    fp.open("users.txt");
    string name, iip, iport;
    string line;
    while (getline(fp, line)) 
    {
        size_t found1=line.find(' ');
        name = line.substr(0, found1);
        size_t found2 = line.find(' ', found1+1);
        iip = line.substr(found1+1, found2-found1-1);
        iport = line.substr(found2);
        int port = stoi(iport);
        M[name] = make_pair(iip, port);
    }
    fp.close();
    cout<<"++++++++++++++++++Connected Freinds++++++++++++++++++"<<endl;
    for (auto it=M.begin(); it!=M.end(); it++)
        cout << it->first<<"("<<it->second.first<<" : "<<it->second.second<<")"<<endl;

	fd_set readset, tempset;
	int portno;
   	int maxfd, flags;
	int srvsock, peersock, j, result, result1, sent;
	socklen_t len;
	struct timeval tv;
	char buffer[MAX_BUFFER_SIZE + 1];
	struct sockaddr_in serveraddr;
	char buf[BUFSIZE];

	if (argc != 2) 
	{
		cout<<"usage: "<<argv[0]<<" <port>"<<endl;
		exit(1);
	}
	portno = stoi(argv[1]);

	/* 
	* socket: create the parent socket 
	*/
	srvsock = socket (AF_INET, SOCK_STREAM, 0);
	if (srvsock <0) 
		error("ERROR opening socket");
	/* setsockopt: Handy debugging trick that lets 
	* us rerun the server immediately after we kill it; 
	* otherwise we have to wait about 20 secs. 
	* Eliminates "ERROR on binding: Address already in use" error. 
	*/
	int optval = 1;
	setsockopt(srvsock, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof (int));
	/*
	* build the server's Internet address
	*/
	bzero((char *) &serveraddr, sizeof (serveraddr));
	/* this is an Internet address */
	serveraddr.sin_family = AF_INET;

	/* let the system figure out our IP address */
	serveraddr.sin_addr.s_addr = htonl (INADDR_ANY);

	/* this is the port we will listen on */
	serveraddr.sin_port = htons ((unsigned short) portno);

	/* 
	* bind: associate the parent socket with a port 
	*/
	if (bind (srvsock, (struct sockaddr *) &serveraddr, sizeof (serveraddr)) <0)
		error ("ERROR on binding");

	/* 
	* listen: make this socket ready to accept connection requests 
	*/
	/* allow 5 requests to queue up */
	if (listen(srvsock, 5)<0) 
		error ("ERROR on listen");
	cout<<endl<<"Server Running, start conversation....."<<endl;
	string myName;
	int checker=0;
	for (auto it = M.begin(); it!=M.end(); it++) 
	{
		if (it->second.first == "127.0.0.1" && it->second.second == portno) 
		{
			cout <<"You are " << it->first << "." << endl << "Start chating..." << endl;
			myName = it->first;
			checker = 1;
			break;
		}
	}
	if (checker == 0) 
	{
		cout<<"Your name is not in the list. You have to register your name at 'ip=127.0.0.1' with port you are using"<<endl;
		exit(1);
	}
	FD_ZERO (&readset);
	FD_SET (srvsock, &readset);
	FD_SET (0, &readset);
	maxfd = srvsock;
	int timeout = 120;
	while(1)
	{
		tv.tv_sec = timeout;
		tv.tv_usec = 0;
		memcpy (&tempset, &readset, sizeof (tempset));
	  	result = select (maxfd + 1, &tempset, NULL, NULL, &tv);
		if (result == 0) 
		{
			cout<<"Idealness for "<<timeout<<" seconds"<<endl;
			exit(1);
		}
		else if (result < 0 && errno != EINTR) 
	  		cout << "Error in select (): " << strerror (errno) << endl;
		else if (result > 0) 
		{
			if (FD_ISSET(srvsock, &tempset)) 
			{
				len = sizeof(serveraddr);
				peersock = accept (srvsock, (struct sockaddr * ) &serveraddr, &len);
				if (peersock < 0) 
					cout << "Error in accept (): " << strerror (errno) << endl;
				else 
				{
					FD_SET (peersock, &readset);
					maxfd = (maxfd <peersock) ? peersock : maxfd;
				}
				FD_CLR (srvsock, &tempset); // changed from tempsock -> tempset
		 	}
		 	if (FD_ISSET (0, &tempset)) 
		 	{
		 		if (read (0, buf, MAX_BUFFER_SIZE) < 0) 
					cout << "Error reading from terminal" << endl;
				if(strlen(buf)>=5 && (buf[0]==101 || buf[0]==113) && (buf[1]==120 || buf[1]==117) && buf[2]==105 && buf[3]==116)
					exit(0);
				int sockfd, portno2, n;
				struct sockaddr_in serveraddr2;
				struct hostent *server;
				string host;
				pair <string, string> personMessage2 = getPersonAndMessage(buf);
				bool isPersonAvailable = false;
				for (auto it=M.begin(); it!=M.end(); it++) 
				{
					if (it->first == personMessage2.first) 
					{
						host= it->second.first;
						portno2 = it->second.second;
						cout << "Sending to : ( " << host << ":" << portno2 << " )" << endl;
						isPersonAvailable = true;
						break;
					}
				}
				if (!isPersonAvailable) 
					continue;
				sockfd = socket (AF_INET, SOCK_STREAM, 0);
				if (sockfd < 0) 
				{
					cout<<"ERROR: opening socket"<<endl;
					exit(1);
				}
				/* gethostbyname: get the server's DNS entry */
				char hostname[50];
				for (int i=0; i<host.length(); i++)
					hostname[i]=host[i];
				hostname[host.length()]='\0';
				//const char *hostname = host.c_str();
				server = gethostbyname(hostname);
				if (server == NULL) 
				{
					cout<<"ERROR, no such host as "<<host<<endl;
					exit(0);
				}

				/* build the server's Internet address */
				bzero ((char *) &serveraddr2, sizeof (serveraddr2));
				serveraddr2.sin_family = AF_INET;
				bcopy ((char *) server -> h_addr, (char *) &serveraddr2.sin_addr.s_addr, server->h_length);
				serveraddr2.sin_port = htons (portno2);

				/* connect: create a connection with the server */
				if (connect (sockfd, (struct sockaddr *) &serveraddr2, sizeof (serveraddr2)) <0)
				{
					cout<<"Error connecting to "<<personMessage2.first<<endl;
				}
				/* send the message line to the server */
				name = myName + ":" + personMessage2.second;
				//const char *message = name.c_str();
				char message[1024];
				for (int i=0; i<name.length(); i++)
					message[i]=name[i];
				message[name.length()]='\0';
				//cout<<message<<endl;
				n = write (sockfd, message, strlen(message));
				if (n < 0) 
					cout<<"ERROR: writing to socket"<<endl;
				close (sockfd);
				continue;
		 	}
			for (j = 1; j <= maxfd; j++) 
			{
				if (FD_ISSET(j, &tempset)) 
				{
					do 
					{
						result = recv (j, buffer, MAX_BUFFER_SIZE, 0);
					} 
					while (result == -1 && errno == EINTR);

					if (result> 0) 
					{
						buffer[result] = '\0';
						cout<<buffer<<endl;
					} 
					else if (result == 0) 
					{
						close (j);
						FD_CLR (j, &readset);
					} 
					else 
					{
						cout << "Error in recv () : " << strerror (errno) << endl;
					}
				} // end if (FD_ISSET(j, &tempset))
			} // end for (j=0;...)
		} // end else if (result> 0)
	}
	return 0;
}

