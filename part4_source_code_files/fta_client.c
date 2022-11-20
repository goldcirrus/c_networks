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
extern int swap_open(unsigned int addr, unsigned short port);
extern int swap_write(int sd, unsigned char buf[], int length);
extern void swap_close(int sd);

// These arguments must be passed to the program
// argv[1]: IP address of server, e.g., "127.0.0.1"
// argv[2]: port number of sever, e.g., "8899"
// argv[3]: input file name(file to be transfered to server)
int main(int argc, char *argv[])
{
	unsigned short server_port;
	unsigned int server_address;
	unsigned char buf[MAXLINE];
	int sd, n, in;

	if (argc < 4) {
		fprintf(stderr, "Usage: %s address port input-filename\n", argv[0]);
		exit(1);
	}

	// set values for server address and port number
	server_address = inet_addr(argv[1]);	// server address
	server_port = htons(atoi(argv[2]));		// port number

	/*connect to the swap server: pass in server IP and server port by command line argument
	 *create a client socket, send one character(0xff) to server. */

	sd = swap_open(server_address, server_port);
	if (sd < 0) {
		fprintf(stderr, "swap_open error\n");
		exit(0);
	}

	//Students to code the following:

	//open input file for reading

	//send input file name o the server

	// loop
		//if there is data in the file, send it to the server
	// end-loop

	// close the connection to the swap server
	swap_close(sd);

	//close connection to input file
	close(in);
}
