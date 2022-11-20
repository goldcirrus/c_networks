/*
*	swap_server.c   Liang Zhong; T00224182
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

int session_id = 0;
int r = 0;	// frame number to receive

int swap_wait(unsigned short port)
{
	/*
	*	if the session is already open, then return error
	*/

	if (session_id != 0)
		return -1;

	/*
	*	accept a connection
	*/

	session_id = swap_accept(port);	// in sdp.o

	/*
	*	return a ssession id
	*/

	return session_id;
}

int swap_read(int sd,unsigned char *buf)
{
	int	n;
        int     frameLen;
	unsigned char	frame[MAXFRAME];
        unsigned char   ack[5];
        unsigned char   checkRcv;
        unsigned char   cheSumOut;
        unsigned char   sequenceR;

	if (session_id == 0 || sd != session_id)
		return -1;

	/*  receive a frame without a timer: write received message into frame
         *  both frame and the passed in parameter:buf is a pointer, can be changed */
        sdp_receive(sd,frame);                        
	
        sequenceR=r+'0';     //conver int 0 to unsigned char for frame sequence# check
        
        frameLen=strlen(frame);                    //received frame length
        checkRcv=checksum(frame,frameLen-1);
     
	/* if frame# is expected and checksum is right:GOOD:send ACK with expected next sequence#:r+1 */
     
        if( (frame[0]==sequenceR) && (frame[frameLen-1]=checkRcv) )
        {
              r++;           //ACK expect next frame sequence#
              ack[0]=r+'0';   //convert int to unsigned char
             
              for(int j=1; j<4;j++)
              {   
                  ack[j]=j;         //add stuff in ack message just for checksum computing
              }
              cheSumOut=checksum(ack,4);
              ack[4]=cheSumOut;

              sdp_send(sd,ack,5);
        }

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
