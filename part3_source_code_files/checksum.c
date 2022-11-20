#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Internet Checksum - RFC-1071

/*both sender and receiver need to use checksum() function to match each other's result. */

unsigned short checksum(unsigned char buf[], int length)
{
	int i;                     //16bits
	unsigned short word16;    //16bits
	unsigned int sum32;       //16bits

	if (length % 2)	// if odd(1),return 999: 1 is true, 0 is false.
		return 999;

	sum32 = 0;
	for (i = 0; i < length; i = i + 2) {
		word16 = (buf[i] << 8) + buf[i+1];
		sum32 = sum32 + word16;
		if (sum32 & 0x10000)
			sum32 = (sum32 & 0xffff) + 1;
	}

	sum32 = ~sum32;

	return (sum32 & 0xffff);
}



int main(){



//sender have char array as its original data
    unsigned char ori[100]="0123";
    printf("\t message to be sent is %s \n",ori);

    //convert char array to unsigned short array.
    unsigned short oriSend[100];
    int i;
    for(i=0; i<strlen(ori);i++)
        oriSend[i]=(unsigned short)ori[i];

    /*computer sender's checksum from new unsigned short array which is send out as message to server.*/
    unsigned short chSend=checksum(oriSend,4);

    /*Sender adds checksum to oriSend data array which is sent out as the message*/
    oriSend[4]=chSend;

    printf("unsigned short checksum is %u \n",oriSend[4]);

//assume message orisend[100] is transmited out as a message
//Receiver receives unsigned short data array, computer its checksum
    unsigned short chRCV=checksum(oriSend,4);
    printf("receiver computes checksum as: %u \n", chRCV);

//compare the receiver computed checksum with received checksum in message
    if(chRCV==oriSend[4])
        printf("sender's checksum matches receiver computed checksum: no error");
    else
        printf("sender's checksum does NOT match receiver computed checksum: error occurs");

    //convert unsigned short oriSend[100]'s data to string
    unsigned char rcvMsg[100];
    for(int j=0; j<sizeof(oriSend);j++)
        rcvMsg[j]=oriSend[j];

    printf("\n\t received message is %s",rcvMsg);

    return 0;
}


