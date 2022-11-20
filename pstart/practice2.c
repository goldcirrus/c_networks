

#include <stdio.h>
#include <stdlib.h>
//the four header files below are necessary to use file i/o functions. 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{   
//    printf("%d arguments. \n", argc);
  //  for(int x=0;x<argc;x++)
    //    printf("\t %s \n",argv[x]);
    
    int in,out;

    in = open(argv[1], O_RDONLY);
    
    if(in<0)
    {
        printf("Cannot open the file %s \n", argv[1]);
        exit (1);
    }

    out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

    if(out<0)
    {
        printf("Cannot create the file %s \n", argv[2]);
        exit (1);
    }

    close(in);
    close(out);
    
    

}
