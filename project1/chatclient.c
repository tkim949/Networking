/*
* * Project Assignment1 - Chat between server and client
* * CS 372 - 2020 Winter 
* * Taekyoung Kim - Due date: Feb. 9. 2020
* * kimtaeky@oregonstate.edu
* * Description: this program creates a simple chat program between client and server
* * The server program is written by python 3 and the client program is written by c.
*/

#include <stdio.h> //fprintf(), printf(), stderr, perror()
#include <stdlib.h> // malloc(), free(), exit()
#include <unistd.h> //chdir(), fork(), exec(), pid_t ssize_t
#include <string.h> // strcmp(), strtok()
#include <sys/types.h> //waitpid()
#include <sys/socket.h> //sa_family_t
#include <netinet/in.h> //in_addr_t, sa_family_t, in_port_t
#include <netdb.h> //h_addrtype, h_length,struct hostent*
#include <arpa/inet.h> //inet_pton(), inet_ntop()


void createChat(int listenSocketFD, char* handle );
int validCheck(char * handle);
void createChat(int listenSocketFD, char* handle )
{
	char message[1024];
	char chat[1024];
	char chatFromSev[1024];
	size_t mes_len = 0;
	int recStatus;
	int sendStatus;
	
	while(1) {
		
		//show the handle and prompt a use to input message!
		printf("%s> ", handle);
		fflush(stdout);
        fflush(stderr);
		fgets(message, sizeof(message), stdin);
		
		message[strcspn(message, "\n")] = '\0'; //get rid of a new line.
		mes_len = strlen(message); //to get rid of white space
		while (mes_len > 0 && (message[mes_len - 1] == ' ' || message[mes_len - 1] == '\t')){ message[--mes_len] = '\0';}
		 //When a client do not input anything, give a message!
		if(mes_len == 0) {
			printf("You did not input anything. Please input!\n");
		     continue; }
			 
		if (mes_len > 500) {
			printf("Your message must not exceed 500 characters.\n");
		    continue;}
		//the client chat format should be "handle> message"	
		memset(chat, '\0', sizeof(chat));
		strcat(chat, handle);
		strcat(chat,"> ");
		strcat(chat, message);
		
		//When a client input "\quit"
		if(strcmp(message, "\\quit") == 0) { //first \ means escape
			
			printf("Client want to disconnect. Connection closing....\n");
			break;
			
		}
		//sending the chat
        sendStatus = send(listenSocketFD, chat, strlen(chat), 0);
		
		if(sendStatus < 0) {fprintf(stderr, "Error on sending client's message.\n"); exit(1); }
        
		//receiving server message
        memset(chatFromSev, '\0', sizeof(chatFromSev));
		printf("Waiting for message from server....\n");
		
		//while(strlen(chatFromSev) == 0) { //at the beginning it is empty.
		recStatus = recv(listenSocketFD, chatFromSev, 512, 0);//message 500 + handle 10 + 1 for < + null
		
		if(recStatus < 0) {fprintf(stderr, "Error on receiving server's message.\n"); exit(1); }
		
		else if(recStatus == 0) {
			printf("Connection closed by server. Closing....\n");
			break;
		}
		else{
			printf("%s\n", chatFromSev); 
		}
					
	}	
    close(listenSocketFD);
    printf("Now connection closed\n");
	
}

int validCheck(char * handle){ // no space and between 1 and 10
	size_t handle_len = strlen(handle);
	char ch = handle[0];
	int i = 0;
	
	//https://www.geeksforgeeks.org/strcspn-in-c/
		while (ch != '\0'){
				ch = handle[i];
				if(isspace(ch)) { //space check!
					
					printf("There must be no space in the handle.\n");
					return 0;
				}
				i++;
			}
		if(handle_len <1 || handle_len > 10){ //check if the handle between 1 and 10
				
				printf("Your name for chatting must be between 1 and 10.\n");
				return 0;
			}
	return 1;
}

int main(int argc, char* argv[]) //char *argv[]
{   //ex)./chatclient flip2.engr.oregonstate.edu 'port number'
	char* fileName = argv[0]; //ex) ./chatclient 
	
	if (argc != 3) { fprintf(stderr, "USAGE: %s plaintext key prt\n", argv[0]); exit(1);}	
	
	char* serverName = argv[1]; //ex)flip2.engr.oregonstate.edu
	char* portN = argv[2];	// ex)  'port number' such as 33345
	char handle[512];	
	int validHandle = 0;
		
	while(validHandle == 0){
		
		printf("Please enter your name for chatting: \n");
		fflush(stdout);
		fflush(stderr);
		fgets(handle, sizeof(handle), stdin);
		//size_t handle_len = strlen(handle);
		
		handle[strcspn(handle, "\n")] = '\0'; 
		
		validHandle = validCheck(handle); //call the function to see if handle with no space and between 1 and 10.
			
	}
	
	/*
	  https://beej.us/guide/bgnet/html/#gethostnameman	
	  http://man7.org/linux/man-pages/man3/getaddrinfo.3.html
	  https://stackoverflow.com/questions/32895440/explain-parameters-of-getaddrinfo
	  https://stackoverflow.com/questions/9449277/compile-error-with-getaddrinfo
	  Also, the lectures from CS344-2019Fall at Oregon State University.
	*/
	//get the address
	struct addrinfo hints;
	struct addrinfo *res;
	memset(&hints, '\0', sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM; // For TCP connection!
	int status;
	if((status = getaddrinfo(serverName, portN, &hints, &res)) !=0) {//
		fprintf(stderr, "Error on address. Please enter the correct port.\n",
				gai_strerror(status));
		exit(1);	
	}
	//create a socket
	int listenSocketFD = socket(res->ai_family, res->ai_socktype, res->ai_protocol); // Create the socket
	
	if (listenSocketFD < 0) {fprintf(stderr, "Error opening socket"); exit(1);}
	
	printf("Try connecting.....\n\n");
	
	//make connection!
	if (connect(listenSocketFD, res->ai_addr, res->ai_addrlen) < 0) {// Connect socket to port from client side
	
	        fprintf(stderr, "Error on  connecting..."); 
			close(listenSocketFD);
			exit(1);
		}
			
	printf("Connected to server on host %s port %d. \n", serverName, portN);
	
	createChat(listenSocketFD, handle); //call the function that sends and gets messages.
		
	return 0;
	
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
