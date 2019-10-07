#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

void do_outputs(FILE *);



int main(int argc, char * argv[])
{   FILE * fpserial;
    struct termios serial;
    if( argc < 2)
    {  printf("You must specify a serial port\n"); exit(-1);
    }
    fpserial=fopen(argv[1],"w+");
    if(!fpserial)
    {
      printf("Failed to open serial port %s\n",argv[1]);
      exit(-2);
    }
    tcgetattr(fileno(fpserial),&serial);
    cfmakeraw(&serial);
    tcsetattr(fileno(fpserial),TCSANOW,&serial);
    printf("If you have not done so already: \n");
    printf("     1) This program should be running in the same directory as the simulator\n");
    printf("     2) You should have a directory in this directory that is ./i_o_directory\n");
    printf("     3) You should have a ramdrive mounted to that directory\n");
    printf( "      mount -t tmpfs -o size=64k tmpfs ./i_o_directory\n");
    printf(" You only need to do this once per boot\n");
    
    

    while(1)
    {
     
      do_outputs(fpserial);
//      usleep(100);
    }
}



void do_outputs(FILE * fpserial)
{
   FILE * fpfile;
   char *file="./i_o_directory/b3_outputs.txt";
   int value=1234;
   static int initialized=0;
   static int last_value=1234;

   fpfile=fopen(file,"r");
   if(fpfile)
   {
      if(fscanf(fpfile,"LEDs:%i",&value))
      { value=(~value)&0xff;
        fclose(fpfile);
           //printf("value is %x oldvalue is %x\n",value,last_value);
        if(!initialized){fprintf(fpserial,"W 0x24 0xff\n");initialized=1;}
        if(value!=1234) if(last_value != value){fprintf(fpserial,"W 0x25 0x%x\n",value&0xff); last_value=value;fflush(fpserial);}
       }
      else
      {
         fclose(fpfile); //catch it next time
         printf("Opened the file, didn't find a value\n");
      }
   }
   else printf("Failed to open file");


}
