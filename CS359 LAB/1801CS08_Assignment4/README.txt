Name - Ammaar Ahmad
Roll number - 1801CS08
CS359 Assignment 4

Timeout is implemented: Idleness limit-120 seconds after that client is closed

Commands ->

open terminal 1 and type these commands:

1. g++ server.cpp -o user1
2. ./user1 4141

Server Running, start conversation.....
You are user1.
Start chating...
user2:hello

user5:gmail

user4/send me a file
Sending to : ( 127.0.0.1:8989 )
exit



open terminal 2 and type these commands:

1. g++ server.cpp -o user2
2. ./user2 7777

Server Running, start conversation.....
You are user2.
Start chating...
user1/hello
Sending to : ( 127.0.0.1:4141 )
user3:hello

user3:I am fine

user2/ok
Sending to : ( 127.0.0.1:7777 )
user2:ok
lo

user4:hi amish

exit



open terminal 3 and type these commands:

1. g++ server.cpp -o user3
2. ./user3 8080

Server Running, start conversation.....
You are user3.
Start chating...
user2/hello
Sending to : ( 127.0.0.1:7777 )
user4:hello

user2/I am fine
Sending to : ( 127.0.0.1:7777 )
Idealness for 120 seconds

//Timeout limit is 120 seconds


open terminal 4 and type these commands:

1. g++ server.cpp -o user4
2. ./user4 8989

Server Running, start conversation.....
You are user4.
Start chating...
user3/hello
Sending to : ( 127.0.0.1:8080 )
user5/hello
Sending to : ( 127.0.0.1:4545 )
user1:send me a file

user2/hi amish 
Sending to : ( 127.0.0.1:7777 )
user5:how are you


open terminal 5 and type these commands:

1. g++ server.cpp -o user5
2. ./user5 4545

Server Running, start conversation.....
You are user5.
Start chating...
user1/gmail
Sending to : ( 127.0.0.1:4141 )
user4:hello

user5/what time it is
Sending to : ( 127.0.0.1:4545 )
user5:what time it is

user4/how are you
Sending to : ( 127.0.0.1:8989 )
exit

Steps to chat:

1. if user<i> want to send message to user<j> then type "user<j>/<message>" in user<i> terminal
   example command: user<j>/hello friend

