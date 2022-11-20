/*
*	swap_client.c
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

int session_id = 0;
int s = 0;	// frame number sent

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

int swap_write(int sd, unsigned char *buf, int length)
{
        int nRcv;
        int numSent;
        int i;
        int buflen;
        int acklen;
        unsigned char frame[MAXFRAME];
        unsigned char checkSend;
        unsigned char ack[MAXFRAME];
        unsigned char checkAck;    

        if (session_id == 0 || sd != session_id)       //if not connect() successfully or sd not be next session_id
                return -1;

        /* send a DATA frame: message in buf is to be sent to remote socket, plus session_id */
        frame[0] = s+'0';              //frame sequence number is added as the 1st element of frame array
        
        for(i=0;i<length;i++)       //length==10: the rest characters in frame are data message from network layer
        {
             frame[i+1]=buf[i];    //only add char in buf[0]-buf[9] to f[1]-f[10]; f[0] is for frame# 
        }
        
       
        printf("\nframe to be sent with f# at head without checksum is %s; buf:%s \n",frame,buf);

                       //compute checksum value for f[0]-f[10]:(including head s#):length+1=11
        checkSend = checksum(frame,length+1);     
                              //add checksum value to the end of frame. f#+data:f[0]-f[10], checksum:f[11]
        frame[length+1]=checkSend;       //length==10:f[11]=checksum;
                                        
           /* send frame out to the server */
        while(1){
              numSent=sdp_send(sd, frame,length+2 );       //n=#char sent=length+2;extra head:s; tail:checksum
        
              s++;        //next frame sequence#, if good:ACK with next 's' is received within timeout
        
       /*  read a ACK frame with a timer: 
        *  expecting ACK with next frame sequence#:s. if received ACK within timeout, send next frame with frame
        *  sequence#:s. If not receive ACK within timeout, resend last frame.  */
       
              nRcv= sdp_receive_with_timer(sd,ack,2000);
              printf("receive raw ACK from server: %s",ack);

              if(nRcv>0){        //receive ACK within timeout, no other errors
        /*compute received ack's checksum, if wrong, drop it and rend last frame. */
                    
                    checkAck=checksum(ack,4);    //checksum only computer first 4 char       
     
       //if checksum matches sender's checksum and ACK has right next frame#-->send another frame by calling this function again
                    if(checkAck==ack[4] && ack[0]==(s+'0'))           //ack pattern is f#,3 char,checksum
                    {     
                         printf("\nACK and Checksum received correctly from server. To send another frame\n");
                         return nRcv;         //return the length sent, and stop loop, terminate this function. 
                    }               
                    if(checkAck==ack[4] && ack[0]==(s-1+'0'))     //if receive duplicated ACK
                    {     
                         printf("Receive ACK expect this frame again. Send this frame again.");
                         s--;           //retrun 0; terminate this funtion, start new write again
                    }
                          
                   
                               //frame# or checksum wrong: back to loop to send same frame again.   
              }                        //reduce frame# s becasue it will increment in the start of loop again
              
              s--;     //not recevie any ACK frame, send this frame again. 
              printf("\n Timeout Fail to receive ACK! Resend the frame again. \n");  
        }              //any other errors by sdp_receive(): resend last frame again from while loop again.

}



void swap_close(int sd)
{
	if (session_id == 0 || sd != session_id)
		return;

	else
		session_id = 0;

	swap_disconnect(sd);	// in sdp.o
}
