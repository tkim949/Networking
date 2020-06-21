
***Programming Assignment1***
***CS372-2020Winter       ***
***Taekyoung Kim          ***

##### Compile #####

(1) for chatserve.py
python does not need to be compiled.

(2) for chatclient.c
I made a Makefile and added in the file, so just input:

$ make


##### Run/Execute #####

(1) for chatserve3.py

$ python3 chatserve3.py PORTNUM

----I have two server files: ----
    chatserve3.py --- this is an advanced file with multi-threading.
    chatserve.py --- this is a basic file with the basic requirements
	(python3 chatserve.py)---for the case when you want to run this.
	

(2) for chatclient.chatclient

$ ./chatclient HOST PORTNUM


(ex: ./client flip2.engr.oregonstate.edu 33345)
The OSU server will give you message "port must be 0-65535" if you use bigger number than 65535.
HOST is server's address and PORTNUM should be the same on client and server sides.

##### Extra Credit - Multi-threading #####
I did multi-threading!
More detail is in the 'Control' below.


##### Control #####

(1) First run chatserve3.py
Then, it shows "Now server is listening...."

(2) Then, run chatclient.c
It will ask you to input your name/handle for the chatting.
Please input a proper name. Otherwise, it gives error message and asks again.

(3) Now the client Host, B and the server Host, A will be connected and show it.

(4) Starting the chat from the client. This chat will take turns.
client sends message -> server gets message -> server sends message ->client gets message (repeat!)

(5) If you want to quit/disconnect, input '\quit' either on Host A or B.
The client side will be disconnected and exit the program, but the server side does not exit and will be waiting for next client

(6) End server program
click "ctrl + c" on the keyboard

(7) EC)multi-threading
You can make more than 1 clients up to 5.
For example) Host A- server, Host B- client1, Host C- client2.
You can make more clients just by running chatclient.c file again on a new terminal.
Then, you can make connections and do chatting with multiple clients simultaneously.
Most of all, a client can send a chat while the server is waiting.
Sign of server is waiting:
         "Sending message. Now waiting for client message...."
         "Waiting for client....."
Other features are just the same as (1) through (6).
   a)run chatclient.c again on a new terminal while existing another chatting is going on.
   b)make a connection while server is waiting
   c)keep chatting and sending should be made while the server is waiting
   d)If the client or server want to quit/disconnect, just follow 5) and 6)


##### Clean chatclient #####

$ make clean
