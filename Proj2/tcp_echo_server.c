//TCP Echo server program
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define	BUFLEN	512			// Maximum length of buffer
#define PORT	8899			// Fixed server port number

int main (void)
{
   /*declare and define listen socket(connect establish) and socket(data transmit). */
	struct sockaddr_in server_address;    // declare a server(local) address variable of socket address struct
	struct sockaddr_in client_address;   // declare a client(remote) address variable of socket address struct
	int client_address_len = 0;           //length of client address(need to be explicitly defined in function)
        int listen_socket;                          //listen socket descriptor(connection establishment phase)
        int s_socket;                          //socket descriptor(reference for data transmit)
        
    /*declare buffer for server. */
        char buffer[BUFLEN];                     //declare a 'buffer' to hold received characters for server
        char* ptr=buffer;                     //a pointer points to the next bytes(char) to receive in 'buffer'
        int len=0;                         //number of bytes(chars) to receive(consume) or send(produce) in buffer
        int maxLen=BUFLEN;            //Maximum number of bytes(characters) allowed in 'buffer'
        int n=0;                               //number of bytes(chars) received for each recv() call
        
   /*define how many client's requests can be in the waiting queue. */
        int waitSize=16;                       //size of waiting clients
		
	
/* 1.Populate/Create server(local) sockets address for both listen_socket and socket */
	
        memset (&server_address, 0, sizeof (server_address));	// Initialize:assign memory to the server address
	server_address.sin_family = AF_INET; 		       // Populate family field - IPV4 protocol
	server_address.sin_port = htons(PORT);	            // Set port number;htons() converts 9988 to binary
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);	    // Set IP address to IPv4 for loacalhost(default any)
	 	
/*2.Create a TCP server's listen socket; returns -1 on failure */
	if ( (listen_socket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP) ) == -1)
        {
		printf("Error: Listen socket failed!\n");
		exit (1);
	}

/*3.Bind the server's listen socket to the server's socket address; returns -1 on failure */
	if ((bind(listen_socket, (struct sockaddr *)&server_address, sizeof (server_address))) == -1)
        {
		printf("Error: binding failed!\n");
		exit (1);
	}
	
	printf("Server is listening, waiting to connect. we will continue ...\n");
	
//student's cods
/*4.Start to listen to connection requests from clients by calling listen() system call. */
        if(listen(listen_socket,waitSize) == -1) 
        {
               printf("Error:listening failed!");
               exit(1);
        }
	
/* Forever in while loop to listen and accept connections request from client...*/
        while(1)    
        {
               /*accept a connection from a client socket by accept() system call. */
              if((s_socket = accept(listen_socket,(struct sockaddr*)&client_address,(socklen_t*)&client_address_len))==-1)
               {
                      printf("Error: accepting failed! Can't open a socket to accept data transmit.");
                      exit(1);
               }

               printf("\n\tTell: listen_socket accepts a client request return a socket for data transmit. ");
               printf("\tclient connected with client's ip address at %s\n",inet_ntoa(client_address.sin_addr));
              

              /*keep receiving as long as the current client keeps the connection */
               while((n=recv(s_socket,ptr,maxLen,0))>0)
               {
                      printf("Tell: received message segment from a client is %s. \n",buffer);
                      ptr+=n;    //move pointer to next postion for next bytes to be received along buffer
                      maxLen-=n;      //decrement maximum bytes to be received 
                      len+=n;         //increment received bytes 
                      
                      send(s_socket,buffer,len,0);   /*!!!!!!!!!!!!!!!!must put send() inside while loop!!!!!*/
               }
              
               /*the code:printf() below ONLY executed after the client disconnect with the server's data s_socket.[recv() returns -1 --> outof the above while loop]. As long as the client is connected to the server, the above while loop runs forever. The client break the connection by [control+c] at client end.  */

               printf("\n\nFinal: outside of recv() working zone: Buffer content to be sent out is %s:\n", buffer);
        
/*!!!Textbook put send(s_socket,buffer,len,0); here, outside of loop where recv() is working! Wrong! Wrong! if put send() function here, it won't work! Because send() only works when recv() is working, and recv() only works insideits while loop. Here is where the recv() returns -1 means it is already stop working. */

//don't close listen_socket in endless while loop because server needs to listen all the time for new clients.
//close(listen_socket) here will cause server terminate listening when the first client finish its turn

               close(s_socket);   
 /*close data transmit socket for the current cient. A new client will inform the listen socket to open a new data transmit socket via accept() function. Then new recv() loop starts. */      
         }     	


		
         close(listen_socket);	    //server quit listening: close descriptor referencing server's listen socket
	 return 0;
}                                              // End main
