NAME : AMMAAR AHMAD
ROLL NO : 1801CS08

For Go Back N
1)open a terminal and start the server 
compile : gcc gbn_client.c -o client
execute: ./client
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
4 I like ML					//This message is discarded due to incorrect sequence number
3 I am good at programming			//This message is discarded due to unexpected sequence number
Acknowledgement recieved to send packet with sequence No. 0

0 I like ML
1 I am good at programming
Acknowledgement recieved to send packet with sequence No. 2


2)open another terminal and start the client program.
compile : gcc gbn_server.c -o server
execute : ./server
Number of packets: 6
Window Size is 3
Acknowledgement sent
Hello
My name is Ammaar Ahmad
BTech 3rd Year Student
Computer Science
I like ML
I am good at programming


For Selective Repeat

1)open a terminal and start the server 
compile : gcc sr_client.c -o client
execute : ./client

Sample Output:
Enter the total number of packets to be sent
6
Enter the window size of buffer
4
Note: Valid Sequence Numbers are from 0 to 7
Enter the sequence number followed by message to be sent for every packet
Acknowledgement recieved to send package with sequence No. 0
0 Hello
1 I am Ammaar Ahmad
2 Interested in ML
2 Interested in Coding
Incorrect Sequence Number. Please enter the sequence followed by message again
3 Interested in Coding
Acknowledgement recieved: Frame 0 recieved succesfully
4 Incorrect Sequence Number issue
Acknowledgement recieved: Frame 1 recieved succesfully
5 Bye
Acknowledgement recieved: Frame 2 recieved succesfully
Acknowledgement recieved: Frame 3 recieved succesfully
Acknowledgement recieved: Frame 4 recieved succesfully
Acknowledgement recieved: Frame 5 recieved succesfully


2)open another terminal and start the client program.
compile : gcc sr_server.c -o server
execute : ./server
Number of packets: 6
Window Size of sender is 4
Acknowledgement sent
Hello
I am Ammaar Ahmad
Interested in ML
Interested in Coding
Incorrect Sequence Number issue
Bye


