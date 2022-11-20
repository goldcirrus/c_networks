/*
*	swap_client.c   Liang Zhong; T00224182
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define	MAXLINE 128	// maximum characters to receive and send at once
#define	MAXFRAME 256

extern int swap_connect(unsigned int addr, unsigned short port);
extern int swap_disconnect(int sd);
extern int sdp_send(int sd, char *buf, int length);
extern int sdp_receive(int sd, char *buf);
extern int sdp_receive_with_timer(int sd, char *buf, unsigned int expiration);
extern unsighed short checksum(unsigned char buf[], int length);

int session_id = 0;
int s = 0;	          // frame number sent

int swap_open(unsigned int addr, unsigned short port)
{
	int	sockfd;		// sockect descriptor
	struct	sockaddr_in	servaddr;	// server address
	char	buf[MAXLINE];
	int	len, n;

	/*
	*	if the session is already open, then return error
	*/

	if (session_id != 0)
		return -1;

	/*
	*	connect to a server
	*/

	session_id = swap_connect(addr, port);	// in sdp.o

	/*
	*	return the seesion id
	*/

	return session_id;
}

/*student work: */
int swap_write(int sd, unsigned char *buf, int length)
{
	int n;
	unsigned char frame[MAXFRAME];
        unsigned char checkSend;       
        unsigned char ack[MAXFRAME]; 

	if (session_id == 0 || sd != session_id)       //if not connect() successfully or sd not be next session_id
		return -1;

	/* send a DATA frame: message in buf is to be sent to remote socket, plus session_id */
        frame[0] = s+'0';              //frame sequence number is added as the 1st element of frame array
        for(int i=1;i<= length;i++)       //the rest characters in frame are data message from network layer
        {
             frame[i]=buf[i];
        }        
        
        /*add checksum in the frame's end */
        printf("\n message to be sent without checksum is %s \n",frame);

              //compute checksum value for message(including head s#)
        checkSend = checksum(frame,length+1);
              //add checksum value to the end of frame
        frame[length+1]=checkSend;

        /* send frame out to the server */
        while(1){
            n=sdp_send(sd, frame,length+2);       //n=#char sent=length+2;extra head:s; tail:checksum
        
            s++;        //next frame sequence#, if good:ACK with next 's' is received within timeout

	/*  read a frame with a timer: 
         *  expecting ACK with next frame sequence#:s. if received ACK within timeout, send next frame with frame
         *  sequence#:s. If not receive ACK within timeout, resend last frame. 	*/
            sdp_receive_with_timer(sd,ack,1000);
        
        //if receive ACK within timeout, return-->send another frame by calling this function again
            if(ack[0]==s)   
                 return n;            //return the length sent
            else
                 s--;        //not received ACK with next s, back to loop to send same frame again.            
          }                 //reduce frame sequence# s because it will increment above again  

}



void swap_close(int sd)
{
	if (session_id == 0 || sd != session_id)
		return;

	else
		session_id = 0;

	swap_disconnect(sd);	// in sdp.o
}
