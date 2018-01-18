#include "simpletools.h"                      // Include simple tools
#include "abdrive.h"
#include "stdio.h"

int main () {
   char str[20];
   printf("Enter Charachers: ");
   scanf("%s", str);

   drive_goto(-100, -100);
   for (int i = 0; i < 20 ; i++)
   {
   		if (str[i] == 'a'){
   			drawA();
}

   		else if (str[i] == 'e'){
   			drawE();
}

   		else if (str[i] == 'f'){
   			drawF();
}

   		 else if (str[i] == 'h'){
   			drawH();
}

   		 else if (str[i] == 'i'){
   			drawI();
}

   		 else if (str[i] == 'k'){
   			drawK();
}

   		else if (str[i] == 'l'){
   			drawL();}
		else{ printf(" ");



   		}

   		
   }
   
   return(0);
}

void drawA()
{
	drive_goto(10, 10);
	drive_goto(25, -26);
	drive_goto(15, 15);
	drive_goto(-25, 26);
  	simulator_startNewSmokeTrail();
	drive_goto(20, 20);
	simulator_stopSmokeTrail();
	drive_goto(-8, -8);
	drive_goto(18, -18);
	drive_goto(45, 45);
  	simulator_startNewSmokeTrail();
	drive_goto(-60, -60);
	simulator_stopSmokeTrail();
	drive_goto(-7, -7);
	drive_goto(15, -15);
	drive_goto(7, 7);
  	simulator_startNewSmokeTrail();
	drive_goto(61, 61);
	simulator_stopSmokeTrail();
	drive_goto(-61, -61);
	drive_goto(-33, 33);
	drive_goto(55, 55);
}
void drawE()
{
	drive_goto(25, -26);
	drive_goto(20, 20);
	drive_goto(-25, 26);	
  	simulator_startNewSmokeTrail();
	drive_goto(20, 20);
	simulator_stopSmokeTrail();
	drive_goto(-27, -27);
	drive_goto(25, -26);
	drive_goto(20, 20);
	drive_goto(-25, 26);
	drive_goto(7, 7);
  	simulator_startNewSmokeTrail();
	drive_goto(20, 20);
	simulator_stopSmokeTrail();
	drive_goto(-27, -27);
	drive_goto(-25, 26);
	drive_goto(7, 7);
  	simulator_startNewSmokeTrail();
	drive_goto(40, 40);
	simulator_stopSmokeTrail();
	drive_goto(-9, -9);
	drive_goto(25, -26);
	drive_goto(7, 7);
  	simulator_startNewSmokeTrail();
	drive_goto(20, 20);
	simulator_stopSmokeTrail();
	drive_goto(20, 20);
}
void drawF()
{
	drive_goto(25, -26);
	drive_goto(20, 20);
	drive_goto(-25, 26);
	simulator_startNewSmokeTrail();
	drive_goto(20, 20);
	simulator_stopSmokeTrail();
	drive_goto(-27, -27);
	drive_goto(25, -26);
	drive_goto(20, 20);
	drive_goto(-51, 51);
	drive_goto(7, 7);
  	simulator_startNewSmokeTrail();
	drive_goto(40, 40);
	simulator_stopSmokeTrail();
	drive_goto(-9, -9);
	drive_goto(25, -26);
	drive_goto(7, 7);
  	simulator_startNewSmokeTrail();
	drive_goto(20, 20);
	simulator_stopSmokeTrail();
	drive_goto(20, 20);
}
void drawI()
{
	simulator_startNewSmokeTrail();
	drive_goto(14, 14);
	simulator_stopSmokeTrail();	
	drive_goto(-15, -15);
	drive_goto(25, -26);
	drive_goto(9, 9);
	simulator_startNewSmokeTrail();
	drive_goto(40, 40);
	simulator_stopSmokeTrail();	
	drive_goto(-9, -9);	
	drive_goto(-25, 26);
    simulator_startNewSmokeTrail();
	drive_goto(14, 14);
	simulator_stopSmokeTrail();	
	drive_goto(-25, 26);
	drive_goto(40, 40);
	drive_goto(25, -26);
	drive_goto(20, 20);
}
void drawL()
{
	drive_goto(25, -26);
	drive_goto(8, 8);
    simulator_startNewSmokeTrail();
	drive_goto(40, 40);
	simulator_stopSmokeTrail();	
	drive_goto(-9, -9);	
	drive_goto(-25, 26);
	drive_goto(8, 8);
	simulator_startNewSmokeTrail();
	drive_goto(20,20);    
	simulator_stopSmokeTrail();	
	drive_goto(-25, 26);
	drive_goto(40, 40);
	drive_goto(25, -26);
	drive_goto(20, 20);
}
void drawH()
{
	
	drive_goto(25, -26);
	drive_goto(8, 8);
	simulator_startNewSmokeTrail();
	drive_goto(40, 40);
	simulator_stopSmokeTrail();	
	drive_goto(-28, -28);
	drive_goto(-26, 25);
	drive_goto(8, 8);
	simulator_startNewSmokeTrail();
	drive_goto(20, 20);
	simulator_stopSmokeTrail();	
	drive_goto(-8, -8);
	drive_goto(-26, 25);
	drive_goto(-12, -12);	
	simulator_startNewSmokeTrail();
	drive_goto(40, 40);
	simulator_stopSmokeTrail();	
	drive_goto(-8, -8);
	drive_goto(25, -26);
	drive_goto(20, 20);
}
void drawK()
{
	drive_goto(25, -26);
	drive_goto(8, 8);
	simulator_startNewSmokeTrail();
	drive_goto(40, 40);
	simulator_stopSmokeTrail();	
	drive_goto(-28, -28);
	drive_goto(-15, 15);
	drive_goto(10, 10);
	simulator_startNewSmokeTrail();
	drive_goto(25, 25);
	simulator_stopSmokeTrail();	
	drive_goto(-33, -33);
	drive_goto(-21, 21);
	drive_goto(10, 10);
	simulator_startNewSmokeTrail();
	drive_goto(25, 25);
	simulator_stopSmokeTrail();	
	drive_goto(11, -10);
	drive_goto(20, 20);
	

}
