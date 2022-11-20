// TCP echo client program; Liang Zhong; T00224182
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE 512	/* buffer size */

int main (int argc, char* argv[ ])		// Three arguments to be checked later
{
        int socket_fd;                               //socket descriptor(reference to client socket)
	
        struct sockaddr_in servAddr;    /*declare server's socket address variable*/
                               /*no need to explicitly define client socket address variable because OS knows it*/
        char buffer[BUFFSIZE];        //buffer holding the received message echoed back from server
        char* ptr=buffer;                    //pointer to move along the buffer for next byte to be received
        int n=0;                                //number of bytes(characters) in each recv() call
        int len=0;                             //length of message to be echoed in buffer     
        int maxLen=BUFFSIZE;             //maximum number of bytes in buffer                              
        

        char *servIP;				// Server IP address from command line
	int servPort;		// Server port number from command line
	char *message;			// Message specified on the command line
        
        

	// Check for correct number of command line arguments
	if(argc != 4) {                                               
		printf("tcp-echo-client [IP address] [Port] [Message]\n");                             
		exit (1);
	}                                                
        
        servIP=argv[1];
        servPort=atoi(argv[2]);
        message=argv[3];

	// Populate/define the remote(server) socket address for the variable 'servAddr'
	memset (&servAddr, 0, sizeof(servAddr));		// Initialize server's socket address variable's data structure(assign memory)
	servAddr.sin_family = AF_INET; 				// server's socket address variable get its faimly struct field: This is an IPv4 address
	servAddr.sin_addr.s_addr = inet_addr(servIP); 	        // Server's socket address variable get its IP address by command line argument input
	servAddr.sin_port = htons(servPort);	         	// server's socket address variable get its port value from command line argument input: Server port number integer converted to binary
	

/* Create a TCP client stream socket: OS knows its socket address fields value, so no need to explictly define its socket address. */
	
	if ((socket_fd = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		printf("Error: socket creation failed!\n");
		exit (1);
	}
	else
        printf("Tell: A client stream Socket is successfully created..\n"); 

	//Establish connection: Connect the client socket to the server socket address defined above
	if ((connect (socket_fd, (struct sockaddr*)&servAddr, sizeof(servAddr))) == -1) {
		printf("Error: connection to the server failed!\n");
		exit (1);
	}
	else
               printf("Tell: Connect() establishment to the server is successful.\n"); 
	
	// Students to complete this section

	// Send data(message read from the command line argument argv[3] to the server...
        send(socket_fd, message, strlen(message),0);        //send message data to client socket
        printf("Student says: message sent out to the server Done successfully. ");

       // Receive data back from the server..
       //while Loop goes forever until the server stop sending data segment to this client.  
        while((n=recv(socket_fd, buffer, maxLen,0))>0)
        {
              ptr+=n;               //move pointer to next position to receive next string segment
              maxLen-=n;            //decrement maximum capacity in buffer
              len+=n;              //increate received number of bytes
              
              buffer[len]='\0';
              printf("echo back is %s \n", buffer);              
        }

        //add '\0' end of string character in the end of string in buffer
      //  buffer[len]='\0';
        
	//print data...
        printf("\nSuccefully get the echo back from server.");
	printf("\nThe echoed back message is  %s.\n", buffer);
        fputs(buffer,stdout); 
    
	// Close socket
	close (socket_fd);
	
	// Stop program
	return 0;
	
} // End main
