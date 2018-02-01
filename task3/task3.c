#include "simpletools.h"                        // Library includes
#include "abdrive.h"
#include "ping.h"  
#include "mylib.h"
int distance;                          // IR variables
int distLeft[4], distRight[4];
int degrees = 90;

int main()                                      // Main function
{
 	low(26);                                      // D/A0 & D/A1 to 0 V
 	low(27);
	drive_getTicks(&distLeft[0], &distRight[0]);
  	drive_setRampStep(12); 
	distance = ping_cm(8);
	if (distance >= 10)
	{
		while(1)
		{	
			drive_getTicks(&distLeft[0], &distRight[0]);
			distance = ping_cm(8);

			goLeftWall();  

			drive_getTicks(&distLeft[1], &distRight[1]);
			if( distLeft[1] - distRight[1] >= 51){
				drive_speed(0, 0);
				break;
			}
		}
	}
	turnToDegree(90);
	while (ping_cm(8) > 7)
	{
		drive_speed(20, 20);
	}
	drive_speed(0, 0);
 	
 }
