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

#define PORT 8899		/* server port # */
#define BUFFSIZE 200	/* buffer size */

int main()
{
	int sockfd;
	int addrlen;       //length of client socket address
	char buffer[BUFFSIZE];
	struct sockaddr_in server;
	struct sockaddr_in client;
	time_t current_time;

	/* Populate socket data structures with IP address and port number */
	/* build server(local) socket address*/
	memset((char *) &server, 0, sizeof(struct sockaddr_in));    //allocate memeory to server socket address
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);   //conver 8899 to binary numbers format

	/* Create an empty UDP socket; returns -1 on failure */
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd == -1) {
		printf("Socket error\n");
		exit(1); /* Exit on error */
	}

	/* Bind the server(local) socket address(server) with the above created empty socket */
	if ((bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr_in))) == -1) {
		printf("Server bind error\n");
		exit (1); /* Exit on error */
	}

	/* Helpful status message */
	printf("Server is listening on port: %d\n", PORT);
	printf("Waiting for client request...\n");
	printf("Press CTRL + C to exit\n");
	
	while(1) {
		addrlen = sizeof(struct sockaddr_in);   /*length of client socket address*/
		/*server receives message from a client, stores this message in buffer, */
		recvfrom(sockfd, buffer,BUFFSIZE, 0,(struct sockaddr *)&client, (socklen_t *)&addrlen);
		current_time = time(NULL);
		sendto(sockfd, (char *) &current_time, (int) sizeof(current_time), 0, (struct sockaddr *)&client, addrlen);
	}
	exit(0);

} /* End of time server program */