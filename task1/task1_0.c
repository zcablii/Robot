#include "simpletools.h"                      
#include "abdrive.h"

int main()                                    
{
  
  drive_goto(26, -25);
  drive_goto(120, 120);
  drive_goto(27, -25);
  drive_goto(150, 150);

   drive_goto(27, -25);  //move to a proper position to begin.
   simulator_startNewSmokeTrail();  //draw trail
  

   //drive_trimSet(AB_FORWARD, AB_LEFT, 50);
   //drive_trim(1);
   drive_goto(308, 308);
   simulator_stopSmokeTrail();
   drive_goto(-7, -7);                
   drive_goto(26, -25);
   drive_goto(7, 7);
   simulator_startNewSmokeTrail(); 
   drive_goto(308, 308);
   simulator_stopSmokeTrail();  
   drive_goto(-7, -7);  
   drive_goto(27, -25);
   drive_goto(7, 7);
   simulator_startNewSmokeTrail(); 
   drive_goto(308, 308);
   simulator_stopSmokeTrail(); 
   drive_goto(-7, -7);  
   drive_goto(26, -25);
   drive_goto(7, 7); 
   simulator_startNewSmokeTrail(); 
   drive_goto(310, 310);
	simulator_stopSmokeTrail();
drive_goto(30, 30);
   
}
