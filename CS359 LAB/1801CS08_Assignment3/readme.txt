NAME : AMMAAR AHMAD
ROLL NO : 1801CS08

For Go Back N protocol

2)open a terminal and start the server 
compile : gcc gbn_server.c -o server
execute : ./server

3)open another terminal and start the client program.
compile : gcc gbn_client.c -o client
execute : ./client

Output:


Client:
Enter the total number of packets to be sent
6
Enter the window size of buffer
3 
Note: Valid Sequence Numbers are from 0 to 3
Enter the sequence number followed by message to be sent for every packet
Acknowledgement recieved to send package with sequence No. 0
0 Hello
1 My name is Ammaar Ahmad
2 BTech 3rd Year Student
Acknowledgement recieved to send packet with sequence No. 3

3 Computer Science
4 I like ML					//This message is discarded as it is incorrect sequence no
3 I am good at programming			//This message is discarded due to expected sequence no = 0
Acknowledgement recieved to send packet with sequence No. 0

0 I like ML
1 I am good at programming
Acknowledgement recieved to send packet with sequence No. 2



For UDP

1)create a text file and write some data in it

2)open a terminal and start the server 
compile : gcc UDP_server.c -o server
execute : ./server

3)open another terminal and start the client program.
compile : gcc UDP_client.c -o 
execute : ./client
Enter the filename //To read by client 
file.txt
Enter the another filename in which data is copied //To write by server
newfile.txt


