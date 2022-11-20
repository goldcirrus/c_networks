#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define	MAXLINE	128	// maximum bytes to receive and send at once

// External functions
extern int swap_wait(unsigned short port);
extern int swap_read(int sd, unsigned char buf[]);
extern void swap_close(int sd);

// pass in:argv[1]: port number, e.g., "8899"; only need to pass in server port number
int main(int argc, char *argv[])
{
	unsigned short server_port;
	unsigned int server_address;
	unsigned char message[MAXLINE];
	int sd, n, out;     //out is output file's file descriptor

	if (argc < 2) {
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(1);
	}

	// set values for server address(no need here) and port number(by command line argument)
	server_port = htons(atoi(argv[1]));

	// connect to the port allocated to the swap server
	//swap_wait(sever_port_number) in swap_server.c
	//sd is a session ID returned by swap_wait();
	//under this sd# sever is waiting to connect with a client at a sever_port#
	sd = swap_wait(server_port);
	if (sd < 0) {
		fprintf(stderr, "swap_open error\n");
		exit(0);
	}

	//Students to code the following:

	//read in the first message as the new name of the file

	//open output file for writing

	//begin-loop
		//get the file data from the client until end-of-file
	//end-loop



	// close the connection to the swap server
	swap_close(sd);

	//close connection to output file; out is output file's file descriptor
	close(out);
}
