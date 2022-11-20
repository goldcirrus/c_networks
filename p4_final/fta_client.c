/*client to read 10 characters at one time and sent it to server*/

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

int main(int argc, char *argv[])
{
	unsigned short server_port;
	unsigned int server_address;
	unsigned char buf[MAXLINE];
        
        int fileP;
 //       FILE *rFp;	
        char *readFile;    //string to hold file name(to be read) by commandline  argv[3]
        int sd, n, in;

         
	if (argc < 4) {
		fprintf(stderr, "Usage: %s address port input-filename\n", argv[0]);
		exit(1);
	}
	
	// set values for server address, port number, name of the file to be read
	server_address = inet_addr(argv[1]);	// server address
	server_port = htons(atoi(argv[2]));		// port number
        readFile=argv[3];                    //name of the file to be read 

	// connect to the swap server
	sd = swap_open(server_address, server_port);
	if (sd < 0) {
		fprintf(stderr, "swap_open error\n");
		exit(0);
	}
  
        	//open file handler for reading
        fileP=open(readFile,O_RDWR,0777);      
        if(fileP==-1)
        {
             printf("Unable to open file: %s", readFile);
             return 1;
        }
        	/*each client's loop:read 10 characters at a time from file */
	while( read(fileP,buf,10) )   //return 0(false) when end of file (non zero integer true) 
        {   //as long as there are next 10 charcters can be read in file, read() keep reading
             
            
              n=swap_write(sd,buf,10);	  //sent text data in buf to server
              printf( "%d chars read from file into buf:%s\n\n", n,buf);
    
        }
	// end-loop
		
	// close the connection to the swap server		
	swap_close(sd);

	//close connection to input file
       close(fileP);
}
