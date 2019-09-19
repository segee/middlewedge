#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct inputs 
{  int go;
   int ia;
   int ib;
   int op;
};
 
void do_inputs(FILE * );
void do_outputs(FILE *);
struct inputs read_allvalues(FILE *);
int read_go(FILE *);




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

//switch A is on port A  porta 0x22, ddra 0x21, pina 0x20
//switch b is on port F portf 0x31, ddrf 0x30, pinf 0x2f)
//op is port c bits 0-3 portc 0x28, ddrc 0x27, pinc 0x26
// go is port e bit 0 port e 0x2e, ddre 0x2d, pind 0x2c
void do_inputs(FILE * fpserial)
{
   char *file="./i_o_directory/b2_inputs.txt";
   FILE * fpfile;
   int value;

   static int oldvalue=1234;
   static int how_many_the_same=0;
   struct inputs allvalues;
    value=read_go(fpserial);
    
    if(value!=oldvalue)
    {
      allvalues=read_allvalues(fpserial);
      fpfile=fopen(file,"w");
      if(fpfile)
      {
         fprintf(fpfile,"Go:0x%x",allvalues.go); 
         fprintf(fpfile,"IA:0x%x",allvalues.ia);
         fprintf(fpfile,"IB:0x%x",allvalues.ib); 
         fprintf(fpfile,"Op:0x%x",allvalues.op);
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
//switch A is on port A  porta 0x22, ddra 0x21, pina 0x20
//switch b is on port F portf 0x31, ddrf 0x30, pinf 0x2f)
//op is port c bits 0-3 portc 0x28, ddrc 0x27, pinc 0x26
// go is port e bit 0 port e 0x2e, ddre 0x2d, pind 0x2c
int read_go(FILE * fps)
{
  int value;
  static int initialized=0;
  char buffer[200];
  if(!initialized)
   {
    fprintf(fps,"W 0x2d 0\n");    ///all inputs
    fprintf(fps,"W 0x2e 0xff\n");  //enable pullups
    fflush(fps);
    initialized=1;
   }
  fprintf(fps,"R 0x2e\n");
  fflush(fps);
// printf("About to read \n"); 
  rewind(fps); 
 do{ fgets(buffer,200,fps);}while(sscanf(buffer,"R 0x2e %i",&value)==0);  
  //printf("Got the value %x\n",value);  

  //printf("Buffer is %s \n",buffer);
  return (value);
}
//switch A is on port A  porta 0x22, ddra 0x21, pina 0x20
//switch b is on port F portf 0x31, ddrf 0x30, pinf 0x2f)
//op is port c bits 0-3 portc 0x28, ddrc 0x27, pinc 0x26
// go is port e bit 0 port e 0x2e, ddre 0x2d, pine 0x2c
struct inputs read_allvalues(FILE * fps)
{   struct inputs values;
    static int initialized=0;
    char buffer[200];
  if(!initialized)
   {
    fprintf(fps,"W 0x21 0\n");    ///all inputs
    fprintf(fps,"W 0x30 0\n");    ///all inputs
    fprintf(fps,"W 0x27 0\n");    ///all inputs
   
    fprintf(fps,"W 0x22 0xff\n");  //enable pullups
    fprintf(fps,"W 0x31 0xff\n");  //enable pullups
    fprintf(fps,"W 0x28 0xff\n");  //enable pullups
    fflush(fps);
    initialized=1;
   }
   fprintf(fps,"R 0x2c\n");
   fprintf(fps,"R 0x26\n");
   fprintf(fps,"R 0x20\n");
   fprintf(fps,"R 0x2f\n");
 
   fflush(fps);
   rewind(fps)
   values.ib=1234;
       while(values.ib==1234)
       {         fgets(buffer,sizeof(buffer),fps);
	         sscanf(buffer,"R 0x2c %i",&values.go);
	         sscanf(buffer,"R 0x26 %i",&values.op);
	         sscanf(buffer,"R 0x20 %i",&values.ia);
	         sscanf(buffer,"R 0x2f %i",&values.ib);
       }
   return values
}
 
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
