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
	int sockfd;
	int addrlen;
	char buffer[BUFFSIZE] = "GET TIME\r\n";
	struct sockaddr_in server;
	struct sockaddr_in client;
	char *servIP = argv[1];				// Server IP address from command line
	int servPort = atoi(argv[2]);		// Server port number from command line
	time_t current_time;

	/* Check that two arguments were passed on the command line */
    if (argc != 3) {
      printf("Usage: udp-time-client [IP address] [Port] \n");
	  exit(1);
	}

    /* Populate server socket data structure with IP address and port number */
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(servPort);
	server.sin_addr.s_addr = inet_addr(servIP); 
	
	/* Populate client socket data structure with IP address and port number */
	memset((void *)&client, '\0', sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(servPort);
	client.sin_addr.s_addr = inet_addr(servIP); 
	
	/* Create a UDP socket; returns -1 on failure */
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd == -1) {
		printf("Socket error\n");
		exit(1);
	}
     
	/* Helpful status message */
	printf("Client is sending on IP address %s port: %d\n", servIP, servPort);
	
	/* Send time request to server */
	addrlen = sizeof(struct sockaddr_in);
	sendto(sockfd, buffer, (int)strlen(buffer) + 1, 0, (struct sockaddr *)&server, addrlen);
	printf("Request sent to server\n");
	/* Receive time request from server */
	recvfrom(sockfd, (char *) &current_time, (int) sizeof(current_time), 0, (struct sockaddr *)&server, (socklen_t *)&addrlen);
		
	/* Print the time received from the server */
	printf("\n The time received from the server:%s\n", ctime(&current_time));

	exit(0);
	
} /* End of time client program */