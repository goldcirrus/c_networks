/*
*	misc.c
*
*		swap_connect() and swap_accept() should not used together
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/unistd.h>

#define	MAXLINE	256	// maximum characters to receive and send at once
/*server and client object files are separated. they compiled separatly. so global variables are not shared between server and client object files.*/
int sockfd;                     //global variable for either server's socket ID, created&bind in swap_accept() function or client's socket ID created in connect()
unsigned int opponent_addr;     //global variable set to client's IP address by swap_accept()
unsigned short opponent_port;   //global variable set to client's port# by swap_accept()
int sssn_id = 0;                //global variable for session ID, incremented one by swap_accept()

/*return received characters number. */
int sdp_receive(int sd, char *buf)
{
	fd_set rfds;           //fd_set:set of socket descriptors;local variable
	struct timeval tv;
	int retval;

	struct	sockaddr_in	cliaddr;   //remote socket address
	int	len, n;

	if (sd != sssn_id)             //match: sd must be next session ID
		return -1;	// general error

	while(1) {
		while(1) {
			FD_ZERO(&rfds);        //clear a set
			FD_SET(sockfd, &rfds);    //add a sockfd to set
			tv.tv_sec = 5;               //wait(block) for 5 seconds
			tv.tv_usec = 0;
			retval = select(sockfd+1, &rfds, NULL, NULL, &tv);
			if (retval)     //if receive something break
				break;
		}
            //server has a socket descriptor that is always listening
		len = sizeof(cliaddr);
        n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
//recvfrom() get remote socket address and assign it to cliaddr variable
//receive with 5 seconds block, receive from remote socket address, store received message in buf
		if ((unsigned char)buf[0] == 0xfe)  // disconnection from the opponent SWAP(0xfe=254)
			return -2;  // disconnection
//	It is done in sdp_send().
//		if (random() % 100 < 20 && n > 0)
//			continue;  // let's drop it
		if (random() % 100 < 50 && n > 10) {    //only half of n>10 get this and break, another half of n>10 trap in loop
			buf[n-1] = 0xff;     //0xff=255
		}

		break;
	}

	if (n <= 0)
		return -1;	// general error
	else if ((unsigned char)buf[0] == 0xfe)  // disconnection from the opponent SWAP
		return -2;	// disconnected
	else
		return n;
}


// expiration in milliseconds
int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration)
{
	fd_set rfds;               //set of socket descriptors
	struct timeval tv;
	int retval;

	struct	sockaddr_in	cliaddr;     //remote socket address
	int	len, n;

	if (sd != sssn_id)                //sd must be the next session ID
		return -1;	// general error

	while(1) {

		/*
		*	set timer
		*/

		while(1) {
			FD_ZERO(&rfds);                 //clear read set
			FD_SET(sockfd, &rfds);           //add a socket descriptor to read set
			tv.tv_sec = expiration / 1000;       //1000 seconds
			tv.tv_usec = (expiration % 1000) * 1000;	// usec -> microsecond, not millisecond
			retval = select(sockfd+1, &rfds, NULL, NULL, &tv);    //receive message
			if (retval)                               //if receive successfully, break
				break;
			else
				return -3;	// timeout error
		}

		/* receive data from remote socket address*/

		len = sizeof(cliaddr);
		n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
//recevfrom() get remote socket's address and assign it to cliaddr variable
		/*
		*	if disconnected
		*/
// disconnection from the opponent SWAP: disconnect() send buf[0]=0xfe
		if (n > 0 && (unsigned char)buf[0] == 0xfe)
			return -2;

		/* include some errors */

		if (random() % 100 < 20 && n > 0)
			return -3;  // timeout error
		if (random() % 100 < 20 && n > 10) {
			buf[n-1] = 0xff;
		}

		break;
	}

	if (n <= 0)
		return -1;	// general error
	else if ((unsigned char)buf[0] == 0xfe)  // disconnection from the opponent SWAP
		return -2;	// disconnected
	else
		return n;
}

/*send message in buf to a remote socket(10% fail, rest sent out successfully),
 *UDP send whatever message in buf to a client*/
int sdp_send(int sd, char *buf, int length)
{
	struct	sockaddr_in	cliaddr;    //remote socket address
	int	len, n;

	if (sd != sssn_id)         //sd(session ID) must be next session's ID
		return -1;

	// 10% of packets are dropped, send() fail, return number of droped characters (length)

	if (random() % 100 < 10)
		return length;  // send() failed: nothing sent out==let's drop it

	// only 90% sent() works to be sent out to remote socket successfully

    //define remote socket address from global variables set by client'open() or server's accept()
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = opponent_addr;   //global variable, set in swap_connect()
	cliaddr.sin_port = opponent_port;          //global variable, set in swap_connect()

	//send message in buf to remote socket
//printf("sdp_send: before sending, addr=%x, port=%d\n", ntohl(opponent_addr), ntohs(opponent_port));
	n = sendto(sockfd, buf, length, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
//printf("sdp_send: after sending\n");

	return n;  //return the length sent out
}

/*A client use it. swap_client.c uses connect(server_IP,server_port#) to open(server_IP, server_port#), pass in server_IP, server_port#
 create client socket fd, send one character(0xff) to server. */
int swap_connect(unsigned int addr, unsigned short port)
{
	struct	sockaddr_in	servaddr;	// server address
	char	buf[MAXLINE];
	int	len, n;
                //declare a  client socket with global variable sockfd with int type(client and server compliled separately)
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("swap_connect: can not open a socket\n");
		return -1;
	}
               //define server socket address
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = addr;
	servaddr.sin_port = port;

	buf[0] = 0xff;
	//send one character(0xff) to server. print("%x",buf[0]);
	n = sendto(sockfd, buf, 10, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
//n is number of char sendto
	if (n < 0)
		return -1;
	else {
		opponent_addr = addr;      //set to server's IP
		opponent_port = port;      //set to server's port#
		sssn_id++;                 //increment session ID
		return sssn_id;             //return incremented session ID
	}
}

/*a client use it to disconnect with a server, after swap_open(), swap_write()*/
int swap_disconnect(int sd)
{
	if (sd != sssn_id)
		return -1;

	char buf[MAXLINE];

	buf[0] = 0xfe;	// disconnect

	sdp_send(sd, buf, 10);
}

/*server use it. called by swap_wait(sever_port#) in swap_server.c, pass in sever_port#
and return a session ID in swap_server.c file for server.
receive message from a client and store message in its local buf.
set global variable opponent_addr=client's IP address
set global variable opponent_port=client's port#  */
int swap_accept(unsigned short port)
{
	fd_set rfds;                    //fd_set: a set(list) of socket descriptors for reading
	struct timeval tv;              //timeout struct has 2 fields: second,microsecond
	int retval;

	struct	sockaddr_in	servaddr;	// server socket address variable
	struct	sockaddr_in	cliaddr;	// client socket address variable
	char	buf[MAXLINE];
	int	len, n;
//create a socket(global variable) for server because this function is called in server end
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("swap_accept: can not open a socket\n");
		return -1;
	}

//define server's socket address: server IP & server_port#
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = port;
//bind this declared socket to server's socket address variable(IP&Port#)
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		printf("swap_accept: can not bind\n");
		return -1;
	}

	while(1) {
		while(1) { /* Watch/listen sockfd to see when it has input from a client. */

			FD_ZERO(&rfds);            //clear a set
			FD_SET(sockfd, &rfds);      //add a socket descriptor to a set
               /* Wait up to five seconds. */
			tv.tv_sec = 5;    //5 seconds
			tv.tv_usec = 0;    //0 microsecond
			       /*The set listed in readfds(&rfds) is watched to see if characters become available for reading*/
			retval = select(sockfd+1, &rfds, NULL, NULL, &tv);
			/* The timeout argument specifies the minimum interval that select() should
			block waiting for a socket descriptor to become ready. if both fields of the
			timeval structure are zero, then select() returns immediately. (This is useful for polling.) If timeout is NULL (no timeout), select() can block indefinitely.*/
			if (retval)      //in C: 1 is true(select return socket number in set(1), 0 is false
				break;
            /*On success getting input, select() return (1): the number of socket descriptors contained in the set. In this case, (1) socket descriptor in set, 1 is returned: break out of the endless while loop.
            select() return(zero) if the timeout expires, then traped in while loop, do the loop again.*/
		}

		len = sizeof(cliaddr);
		/*receive message from a client, record string in buf*/
		n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
//printf("accept: received, n = %d, %x\n", n, (unsigned char)buf[0]);
        /*client send its first character(0xff) to server via swap_open()-->swap_connect(). */
		if ((unsigned char)buf[0] == 0xff) {
//printf("accept: received connection request\n");
			break;     //received client's first character via its swap_connect(), then break
		}
	}

	opponent_addr = cliaddr.sin_addr.s_addr;     //set to client's IP address
	opponent_port = cliaddr.sin_port;            //set to client's port#
//printf("swap_accept: addr=%x, port=%d\n", ntohl(opponent_addr), ntohs(opponent_port));
//printf("swap_accept: addr=%x, port=%d\n", ntohl(cliaddr.sin_addr.s_addr), ntohs(cliaddr.sin_port));
	sssn_id++;

	return sssn_id;
}


