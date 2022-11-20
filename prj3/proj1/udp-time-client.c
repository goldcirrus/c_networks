/*
 * COMP 3271 - Computer Networks
 * Project Part 1 - UDP Time Server - client implementation
 */
 
#include <stdio.h>		/* I/O functions */
#include <string.h>			/* string functions */
#include <stdlib.h>			/* C standard functions */
#include <sys/socket.h>	/* socket functions */
#include <sys/types.h>	/* library of basic types */
#include <netinet/in.h>	/* library of Internet address functions */
#include <arpa/inet.h>	 	/* Internet operations */
#include <time.h>			/* time functions */

#define BUFFSIZE 200	/* buffer size */

int main(int argc, char *argv[])
{
	int sockfd;   /*socket descriptor(reference to client socket)*/
	int addrlen;    /*server socket address length*/

	char buffer[BUFFSIZE] = "GET TIME REQUEST from Liang Zhong; T00224182\r\n";     
                                               /*buffer containing data(String/char array) sent to the server*/
	char bufferMessageReceived[BUFFSIZE+100];     //buffer to store data received from the server's message

        struct sockaddr_in server;           //socket address struct of server site
	struct sockaddr_in client;           //socket address struct of client site(not used)
        
        char *servIP = argv[1];	        	// get Server IP address entered as a shell command line parameter
        int servPort = atoi(argv[2]);	     	// get Server port number entered as a shell command line parameter
	time_t current_time;

	/* Check that two arguments(server ip address and port number) were passed on the command line */
    if (argc != 3) {
      printf("Usage: udp-time-client [IP address] [Port] \n");
	  exit(1);
	}

   
        /* Populate(build) server socket address's data structure with IP address and port number */
	memset((char *) &server, 0, sizeof(server));    /*allocate memory to server socket address*/
	server.sin_family = AF_INET;                    /*server's socket address's Family field*/
	server.sin_port = htons(servPort);              /*server port number: entered as command line parameter assigned to server's socket address struct*/
	server.sin_addr.s_addr = inet_addr(servIP);     /*server's IP: entered as a shell command line parameter assigned to server's socket address struct*/
	

        /*client socket has both the server socket address and the client socket address filled*/
	/* Populate(build) the client socket address's data structure with IP address and port number */
        /*not necessary to explictly build the client socket address as the 4 lines of codes below since the OS will do it automatically*/
//	memset((void *)&client, '\0', sizeof(client));   /*allocate memeory to client socket address*/
//	client.sin_family = AF_INET;                    /*client socket address's Family field*/
//	client.sin_port = htons(servPort);              /*client socket address's port number?????serverPort? */
//	client.sin_addr.s_addr = inet_addr(servIP);     /*client socket address's IP???servIP???*/
	
	/* Create an empty UDP socket for a client; returns -1 on failure */
	sockfd = socket(AF_INET,SOCK_DGRAM,0);    /*create a new empty socket refered to the client socket*/
	if (sockfd == -1) {
		printf("Socket error\n");
		exit(1);
	}
     
	/* Helpful status message */
	printf("Client is sending to a server on IP address %s port: %d\n", servIP, servPort);
	
	/* Send time request including my name and student number to server */
	addrlen = sizeof(struct sockaddr_in);         /*server socket address's length in bytes*/
	
/*transmit data between client socket and server socket: server sclient's message is stored in buffer, destination socket address is provided with &server*/

        sendto(sockfd, buffer, (int)strlen(buffer) + 1, 0, (struct sockaddr *)&server, addrlen);

	printf("Request sent to server\n");

	/*the client receives the returned message from server and store the returned message in bufferMessageReceived*/

	recvfrom(sockfd, bufferMessageReceived,(strlen(buffer)+100), 0, (struct sockaddr *)&server, (socklen_t *)&addrlen);
		
	/* Print the returned message received from the server */
	printf("\n The message received from the server: %s\n", bufferMessageReceived);

        
	exit(0);
	
} /* End of time client program */
