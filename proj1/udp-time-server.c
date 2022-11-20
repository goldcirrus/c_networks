/*
 * COMP 3271 - Computer Networks
 * Project Part 1 - UDP Time Server - server implementation
 */

#include <stdio.h>		/* I/O functions */
#include <string.h>		/* string functions */
#include <stdlib.h>		/* C standard functions */
#include <sys/socket.h>	/* socket functions */
#include <sys/types.h>	/* library of basic types */
#include <netinet/in.h>	/* library of Internet address functions */
#include <arpa/inet.h>	/* Internet operations */
#include <time.h>		/* time functions */

#define PORT 9988		/* server port # */
#define BUFFSIZE 200	/* buffer size */

int main()
{
	int sockfd;      /*socket descriptor(reference to server socket)*/
	int addrlen;     /*length of client socket address*/
        int messagelen;      /*length of message string*/
	char buffer[BUFFSIZE];   /*data buffer to hold data(string) received from a client and returned back to the client*/
	struct sockaddr_in server;    /*server(local) socket address*/
	struct sockaddr_in client;     /*client(remote) socket address*/
	time_t current_time;

	/* Populate socket data structures with IP address and port number */
        /*build server(local) socket address*/
	memset((char *) &server, 0, sizeof(struct sockaddr_in));     /*allocate memory to server socket address*/
	server.sin_family = AF_INET;            /*Family field*/
	server.sin_port = htons(PORT);          /*convert 9988 to binary number format: htons() converts*/

	/* Create an empty UDP socket; returns -1 on failure */
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd == -1) {
		printf("Socket creation has an error\n");
		exit(1); /* Exit on error */
	}

	/* Bind the server(local) socket address:server to the above created empty UDP socket:sockfd. */
       /*server socket only fill the server(local) socket address. It doesn't know the client socket address until          it gets a request from a client*/
	if ((bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr_in))) == -1) {
		printf("Server bind error\n");
		exit (1); /* Exit on error */
	}

	/* Helpful status message */
	printf("Server is listening on port: %d\n", PORT);
	printf("Waiting for client request...\n");
	printf("Press CTRL + C to exit\n");
	
       /*infinite loop for server to run forever:
      1.Server issues a receive request command, which blocks until it receives a request from a client. 
      2.When server receives a request from a lient, unblock, server fills the UDP socket with the client(remote)          socket address, server does something, and server returns a message back to the client
      3.Server starts another iteration starting from 1 again. */
	while(1) {
		addrlen = sizeof(struct sockaddr_in);     /*length of client socket address*/
       /*server receives message(string) from a client socket address, and stores the data(message) in buffer. */
		messagelen=recvfrom(sockfd, buffer,BUFFSIZE, 0,(struct sockaddr *)&client, (socklen_t *)&addrlen);
                
                /*server concatnates current time to the received message from a client*/
		current_time = time(NULL);    /*time() return time_t which is integer to represent seconds*/
                strcat(buffer, ctime(&current_time));     /*ctime(&time(NULL)) converts time_t to a char array(string) and concatnate the string representing the current time to the buffer(contains a client's message)'s string*/

                /*send the string message in buffer back to the client */
		sendto(sockfd, buffer, messagelen+50, 0, (struct sockaddr *)&client, addrlen);
	}
	exit(0);

} /* End of time server program */
