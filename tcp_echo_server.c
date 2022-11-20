//TCP Echo server program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define	BUFLEN	512											// Maximum length of buffer
#define PORT	9988										// Fixed server port number

int main (void)
{
	struct sockaddr_in server_address;						// Data structure for server address
	struct sockaddr_in client_address;						// Data structure for client address
	int client_address_len = 0;
		
	
	// Populate socket address for the server
	memset (&server_address, 0, sizeof (server_address));	// Initialize server address data structure
	server_address.sin_family = AF_INET; 					// Populate family field - IPV4 protocol
	server_address.sin_port = PORT;							// Set port number
	server_address.sin_addr.s_addr = INADDR_ANY;			// Set IP address to IPv4 value for loacalhost
	 	
	// Create a TCP socket; returns -1 on failure
	int listen_sock;
	if ((listen_sock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error: Listen socket failed!\n");
		exit (1);
	}

	// Bind the socket to the server address; returns -1 on failure
	if ((bind(listen_sock, (struct sockaddr *)&server_address, sizeof (server_address))) == -1) {
		printf("Error: binding failed!\n");
		exit (1);
	}
	
	printf("Server not fully implemented...\n");
	// Students to complete this section
	
	// Listen for connections...
	
	// Accept connections from client...
	
	// Receive and format the data...
	
	// Echo data back to the client...
		
	close (listen_sock);									// Cloese descriptor referencing server socket
	
} // End main
