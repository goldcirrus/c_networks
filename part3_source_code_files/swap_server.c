/*
*	swap_server.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define	MAXLINE	128	// maximum characters to receive and send at once
#define MAXFRAME 256

extern int swap_accept(unsigned short port);
extern int swap_disconnect(int sd);
extern int sdp_send(int sd, char *buf, int length);
extern int sdp_receive(int sd, char *buf);
extern int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration);

int session_id = 0;       //global variable
int R = 0;	// frame number to receive

/*test_server.c calling this function to pass in server_port# to it;
 *It is used by a SWAP server to wait for a reliable session connection request from a SWAP client.
 *It returns an identifier(session ID) used for data exchange, created by swap_accept(sever_port#) in sdp.c file
 *sever use this function's returned session ID in swap_read(sd,buf) to exchange data with a client. */
int swap_wait(unsigned short port)
{
	/* if the session is already open(session_id is not zero), then return error */
	if (session_id != 0)
		return -1;

/* if current session_ID is zero, accept a connection, return a session ID
* session_id is global variable, so swap_read(), swap_close() can modify it too. */

    session_id = swap_accept(port);	// in sdp.o; pass in server_port#

	/* return a session id for data exchange */
    return session_id;
}

/*Student work!  Server only read what received in frames.
pass in a session_ID as sd, pass in a message string */
int swap_read(int sd, char *buf)
{
	int	n;
	char	frame[MAXFRAME];

	/*if passed in sd not equal to session_id returned by swap_accept(), error*/
	if (session_id == 0 || sd != session_id)
		return -1;

	/*
	*	receive a frame without a timer
	*/

	// ...

	/*
	*	several cases
	*/

	// ...

	/*
	*	copy the data field in the frame into buf, and return the length
	*/

	// ...
}

void swap_close(int sd)
{
	if (session_id == 0 || sd != session_id)
		return;

	else
		session_id = 0;

	swap_disconnect(sd);	// in sdp.o
}
