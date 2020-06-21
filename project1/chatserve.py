#!/usr/bin/python3

'''
Title: Programming Assignment1
Name: Taekyoung Kim
Date: 2/2/2020
Class: CS372-2020Winter

https://realpython.com/python-sockets/
https://www.youtube.com/watch?v=CV7_stUWvBQ
https://www.youtube.com/watch?v=DIPZoZheMTo
'''


import sys
import socket

# https://stackoverflow.com/questions/7670303/purpose-of-usr-bin-python3
# https://stackoverflow.com/questions/4233598/about-setsockopt-and-getsockopt-function

if __name__ == "__main__":
	if len(sys.argv) != 2: #check the argument of user's input!
		print ("Usage: python chatserve.py port")
		exit(1)
	# python3 chatserve.py 'port number'	
	portNum = sys.argv[1] #argv[0]: file name
    # create a socket
	sevSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # SOCK_STREAM means TCP connection!
	sevSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # for reuse the port number!
	
	# bind
	sevSocket.bind(('', int(portNum))) #serSocket.bind(host, int(portNum))
	
	# listen -- only one connection
	sevSocket.listen(1)
	print("Now server is listening....")
	
	handle ='TKchat' # hardcorded handle
		
	while True:
		connection, address = sevSocket.accept()
		print ("Connected with client on address {}".format(address))
		
		#chat(connection, handler(connection, handle), handle)
		message = ''
		chatMsg = ''
		msgFromClient = ''
				
		# https://stackoverflow.com/questions/43713801/how-to-change-command-line-for-execution
		# https://stackoverflow.com/questions/13979764/python-converting-sock-recv-to-string
		while True:
			msgFromClient = connection.recv(512).decode() #500 for message + 10 for handle + 1 for '<' + 1 for null.
			
			if msgFromClient == '':
				print ("Connection closed by client.Closing......\n")
				print ("Waiting for client.....")
				break
			
			print (msgFromClient) # print the handle & message from client!
			msgFromClient =''
			message=''
			# at the beginning, message='', so the length is 0
			while len(message) > 500 or len(message) == 0:
				message = input(f'{handle}> ')
				message.rstrip('\t')
				#https://stackoverflow.com/questions/954834/how-do-i-use-raw-input-in-python-3
				if len(message) > 500 or len(message) == 0:
					print("Please input greater than 1 and up to 500.")
					
			
			chatMsg = handle + '>' + message #TKchat> message
			
			if message == '\quit':
				print ("Server want to disconnect. Closing....\n")
				print ("Waiting for client......")
				break
			#https://webkul.com/blog/string-and-bytes-conversion-in-python3-x/
			connection.send(chatMsg.encode()) # send server's message with handle!
			print("Sending message. Now waiting for client message....")	
		
		connection.close()
	
	
	
		
		
