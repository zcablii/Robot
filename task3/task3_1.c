#include "simpletools.h"                        // Library includes
#include "abdrive.h"
#include "ping.h"

int irLeft, irRight;                            // IR variables
int distance;  
int main()                                      // Main function
{
  low(26);                                      // D/A0 & D/A1 to 0 V
  low(27);

  drive_setRampStep(12);                        // Max step 12 ticks/s every 20 ms

  while(1)
  {
    freqout(11, 1, 38000);                      // Check left & right objects
    irLeft = input(10);

    freqout(1, 1, 38000);
    irRight = input(2);

    if(irRight == 1 && irLeft == 1)             // No obstacles?
      drive_rampStep(20, 30);                 // ...full speed ahead
    else if(irLeft == 0 && irRight == 0)        // Left & right obstacles?
     drive_rampStep(20, 30);               // ...full speed reverse
    else if(irRight == 0)                       // Just right obstacle?
    drive_rampStep(-5, 5);                // ...rotate left
     else if(irLeft == 0)                        // Just left obstacle?
      drive_rampStep(40, 20);                // ...rotate right
	distance = ping_cm(8); 
	if (distance <= 5)
        {
            drive_speed(0, 0);
        }                             

        if (distance <= 5)
        {
            drive_rampStep(-120, -120); 
        }
    }
}

