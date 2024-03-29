#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
 
void do_inputs(FILE * );
void do_outputs(FILE *);
int read_value(FILE *);

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
      do_inputs(fpserial);
      do_outputs(fpserial);
//      usleep(100);
    }
}
void do_inputs(FILE * fpserial)
{
   char *file="./i_o_directory/b1_inputs.txt";
   FILE * fpfile;
   int value;

   static int oldvalue=1234;
   static int how_many_the_same=0;
   static int how_often=1000;
   if(how_often<10) how_often++;
   else
   {
    how_often=0;
    value=read_value(fpserial);
    value=~value&0xf;
    if(value!=oldvalue)
    {
      fpfile=fopen(file,"w");
      if(fpfile)
      {
         fprintf(fpfile,"Switches:0x%x",value&0xf);
//       printf("value is %x\n",value);
         fclose(fpfile);
         oldvalue=value;
         how_many_the_same=0;
      }
      else printf("Failed to open file");
    }
    else
    {
     how_many_the_same++;
     if(how_many_the_same>100){ oldvalue=1234 ; how_often=1000;}
    }
   }
 }
int read_value(FILE * fps)
{
  int value;
  static int initialized=0;
  char buffer[200];
  if(!initialized)
   {
    fprintf(fps,"W 0x2a 0\n");
    fprintf(fps,"W 0x2b 0xff\n");
    fflush(fps);
    initialized=1;
   }
  fprintf(fps,"R 0x29\n");
  fflush(fps);
// printf("About to read \n"); 
  rewind(fps); 
 do{ fgets(buffer,200,fps);}while(sscanf(buffer,"R 0x29 %i",&value)==0);  
  //printf("Got the value %x\n",value);  

  //printf("Buffer is %s \n",buffer);
  return (value);
}
void do_outputs(FILE * fpserial)
{
   FILE * fpfile;
   char *file="./i_o_directory/b1_outputs.txt";
   int value=1234;
   static int initialized=0;
   static int last_value=1234;

   fpfile=fopen(file,"r");
   if(fpfile)
   {
      if(fscanf(fpfile,"LEDs:%i",&value))
      {
        fclose(fpfile);
           //printf("value is %x oldvalue is %x\n",value,last_value);
        if(!initialized){fprintf(fpserial,"W 0x24 0xff\n");initialized=1;}
        if(value!=1234) if(last_value != value){fprintf(fpserial,"W 0x25 0x%x\n",value&0xf); last_value=value;fflush(fpserial);}
       }
      else
      {
         fclose(fpfile); //catch it next time
         printf("Opened the file, didn't find a value\n");
      }
   }
   else printf("Failed to open file");


}
