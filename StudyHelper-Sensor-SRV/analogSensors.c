#include <wiringPi.h>
#include <mcp3004.h>
#include <stdio.h>
#include <stdlib.h>


#define BASE 200
#define SPI_CHAN 0

int main(void)

{
  int chan;
  int x;
printf ("Raspberry Pi wiringPi test program\n") ;

 if (wiringPiSetup () == -1)
    exit (1) ;

  mcp3004Setup (BASE, SPI_CHAN); // 3004 and 3008 are the same 4/8 channels

  for (chan = 0 ; chan < 8 ; ++chan) {
    x = analogRead (BASE + chan) ;
  printf("%d\n", x);
    }
   return 0 ;
}
