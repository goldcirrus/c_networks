#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Internet Checksum - RFC-1071

/*both sender and receiver need to use checksum() function to match each other's result. */

unsigned char checksum(unsigned char buf[], int length)
{
	int i;                     //16bits
	unsigned short word16;    //16bits
	unsigned int sum32;       //16bits

	if (length % 2)	// if odd(1),return 999: 1 is true, 0 is false.
		return 999;
                       //only works for length is even
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

    /*computer sender's checksum from new unsigned short array which is send out as message to server.*/
    unsigned char chSend=checksum(ori,strlen(ori));

    /*Sender adds checksum to oriSend data array which is sent out as the message*/
    ori[4]=chSend;

    printf("new message with checksum is %s", ori);
    printf("unsigned short checksum is %u \n",ori[4]);

//assume message orisend[100] is transmited out as a message
//Receiver receives unsigned short data array, computer its checksum
    unsigned char chRCV=checksum(ori,strlen(ori)-1);
    printf("receiver computes checksum as: %u. string length is %d \n", chRCV,strlen(ori));

//compare the receiver computed checksum with received checksum in message
    if(chRCV==ori[4])
        printf("sender's checksum matches receiver computed checksum: no error");
    else
        printf("sender's checksum does NOT match receiver computed checksum: error occurs");

    unsigned char x;
    int s=0;
    x=s+'0';
    printf("\nprint out the sequence# int converted to unsigned char: %c",x);

    return 0;
}


