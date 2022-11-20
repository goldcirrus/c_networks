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



int swap_read(int sd, char *buf)
{
        int     n;              //save number of chars received
        int     frameLen;
        int     j;
        int     k;
        unsigned char   frame[MAXFRAME];
        unsigned char   ack[5];
        unsigned char   checkRcv;
        unsigned char   cheSumOut;
        unsigned char   seqF;

        if (session_id == 0 || sd != session_id)
                return -1;

/*  receive a frame without a timer: write received message into 'frame' variable
 *  both frame and the passed in parameter:buf is a pointer, can be changed */

        n=sdp_receive(sd,frame);               //server receive frame from a client
        frameLen=n;       
//        printf("swap_read: %s. frame_len:%d \n ",frame,frameLen );
       
/*strip the frame to get sequence#(head) and checksum(tail) */
      //expected sequence:r+'0': conver int r to unsigned char because frame contains char

                            //received frame length -2:-1 checksum char, -1 '\0': only computer length==11
        checkRcv=checksum(frame,11);       //compute checksum from received frame except sent checksum

 /* if frame# is expected and checksum is right:GOOD:send ACK with expected next sequence#:r+1 */
      //if f# is r or previous r(recevie previous frame again) 
        if( (n>0) && (frame[0]==r+'0') && (frame[11]==checkRcv) )
        {    

              /*populate passed in parameter: buf with data in received frame */
              printf("swap_read() frame length:%d \n",frameLen);
              for(k=1;k<=10;k++)    //frameLen:12, f[1]-f[10]: buf[0]-buf[9] 
                  buf[k-1]=frame[k];         //assign frame's data(strip off head&tail) to buf(parameter)                   
    
              r++;           //ACK expect next frame sequence#
              ack[0]=r+'0';   //convert ACK's next frame# from int to unsigned char, add it to ack head

              for(j=1; j<4;j++)
              {
                  ack[j]=j+'0';         //add stuff in ack message just for checksum computing
              }
              cheSumOut=checksum(ack,4);      //compute sender's checksum value
              ack[4]=cheSumOut;               //add sender's checksum to ack tail

//              printf("send ACK: %s", ack);
              sdp_send(sd,ack,5);       //send out ACK to client with expected next frame sequence#
                            //ack total 5 char, head is next frame# expected to receive, tail is checksum
              return n;
          }

          //if receive f# is NOT expected f#: server send last ack again, don't renew buf
          if((n>0) && (frame[0]!=r+'0') && (frame[11]==checkRcv) )
          {
               //send ACK with r+0' again
               ack[0]=r+'0';
               for(j=1;j<4;j++)
                   ack[j]=j+'0';   
               cheSumOut=checksum(ack,4);
               ack[4]=cheSumOut;
               sdp_send(sd,ack,5);   //send ack to client

               return 1;     //retrun 1 to let fta_server knows doing nothing with message received.              
          }

          else
              {return n;} //else: return n: usually negative n to stop fta_server to swap_read() again.

}

void swap_close(int sd)
{
	if (session_id == 0 || sd != session_id)
		return;

	else
		session_id = 0;

	swap_disconnect(sd);	// in sdp.o
}
