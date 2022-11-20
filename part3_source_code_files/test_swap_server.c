#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_FTA	128

// External functions
extern int swap_wait(unsigned short port);
extern int swap_read(int sd, unsigned char buf[]);
extern void swap_close(int sd);

// These arguments must be passed to the program
// argv[1]: port number, e.g., "6789"
//only need to pass in server port number

int main (int argc, char *argv[])
{
	int	sd, bytes_read;
	char	buf[MAX_FTA];

	//argv[0] is command name, argv[1] is server's port number, total argc is 2
	if (argc < 2) {
		fprintf(stderr, "Usage: %s port_number\n", argv[0]);
		exit(1);
	}

	//swap_wait(sever_port_number) in swap_server.c; pass in sever's port number
	//sd is a session ID returned by swap_wait();sever is waiting to connect with a client at a sever_port#
	sd = swap_wait(htons(atoi(argv[1])));        //pass in sever_port#

	if (sd < 0) {
		fprintf(stderr, "%s cannot wait, %d\n", argv[0], sd);  //sever can't get a message at its port#
		exit(1);
	}

	// read message from the client and store them into the above file
	//use student written function swap_read(,), swap_read(sd,buf) can modify string parameter buf(array is like a pointer)
	while((bytes_read = swap_read(sd, buf)) > 0) {
		buf[bytes_read] = '\0';
		printf("%s\n", buf);
	}

	// close the file and the connection
	swap_close(sd);
}

