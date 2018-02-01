#include "mylib.h"
#include "simpletools.h"
#include "ping.h"
#include "abdrive.h"


void turnToDegree(int degree)
{
	double val = 16.277/(180/PI);
	int ticks = val * degree;
	drive_goto(-ticks, ticks);
}

void goLeftWall()
{
  int irLeft, irRight;                          
	freqout(11, 1, 38000);                      // Check left & right objects
    irLeft = input(10);
    freqout(1, 1, 38000);
    irRight = input(2);

 	if(irRight == 1 && irLeft == 1)             // No obstacles?
      drive_rampStep(16, 24);                 // ...Go ahead
    else if(irLeft == 0 && irRight == 0)        // Left & right obstacles?
      drive_rampStep(48, 48);               // ...full speed reverse
    else if(irRight == 0)                       // Just right obstacle?
      drive_rampStep(12, 24);                // ...rotate left
    else 		              		          // Just left obstacle?
      drive_rampStep(25, 12);  
}


