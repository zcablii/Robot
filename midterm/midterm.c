#include "simpletools.h"                        
#include "abdrive.h"
#include "ping.h"
#include "math.h"

//Declaration of Global Variables
const float w = 105.8;
int leftTicks[100], rightTicks[100], leftTicksDiff[100], rightTicksDiff[100], i=0, cycleCount=1;
float dl, dr, theta, prevtheta = 0.0, thetachange, rl, rr, rm, dy, dx, totaldy, totaldx, hypo;
int irLeft = 0 , irRight = 0;
int errorValue = 0, prevError = 0, errSum = 0, errDiff = 0, corrSpeed = 0; 
int atWall = 0;
int iteration_time = 20;
int stop_dist = 10;
int Kp = 3, Ki = 2, Kd = 1; //K constants for PID controller

struct node {
    int distRight;
    int distLeft;
    struct node* next;
    struct node* prev;
};  

//calculates correction speed using PID equation
int calcSpeedCorrection(int errorValue, int errSum, int errDiff) 
{
  int corrSpeed = (Kp * errorValue) + (Ki * errSum) + (Kd * errDiff);
  //integration with reset windup guarding using pre-determined bounds
  if (corrSpeed > 20) {
     corrSpeed = 20;
   } else if (corrSpeed < -20) {
     corrSpeed = -20;
   }
  return corrSpeed; 
}

//records distance of each wheel using dead reckoning
void recordDist() {
    i++;
   drive_getTicks(&leftTicks[i], &rightTicks[i]);
   leftTicksDiff[i] = leftTicks[i] - leftTicks[i-1];
   rightTicksDiff[i] = rightTicks[i] - rightTicks[i-1];
   dl = leftTicksDiff[i] * 3.25;
   dr = rightTicksDiff[i] * 3.25;
   theta = (dl-dr) / w;
   if (theta==0) {
       dy = dl * sin(prevtheta);
       dx = dl * cos(prevtheta);
       prevtheta += theta;
       totaldy += dy;
       totaldx += dx;
   } else {
       rl = dl / theta;
       rr = dr / theta;
       rm = (rl + rr) / 2;
       dy = rm*sin(prevtheta+theta) - rm*sin(prevtheta);
       dx = rm*cos(prevtheta+theta) - rm*cos(prevtheta);
       prevtheta += theta;
       totaldy += dy;
       totaldx += dx;
   }
} 

int main()                                      
{
  low(26);                                     
  low(27);     

    struct node *begin;
    begin = (struct node *) malloc(sizeof(struct node));
    begin->prev = NULL;
    begin->distLeft = 0;
    begin->distRight = 0;
    
  struct node *previous;
    previous = begin;

  drive_getTicks(&leftTicks[i], &rightTicks[i]);                   

  while(1){
  
  if (!atWall) {

    struct node *current;
    current = (struct node *) malloc(sizeof(struct node));
    //detects left/right obstacles, and stores values between 0 to 20 in irLeft and irRight
    for (int k = 0; k < iteration_time; k++) {
    freqout(11, 1, 38000);                     
    irLeft += input(10);
    freqout(1, 1, 38000);
    irRight += input(2);
    } 
  
    if (ping_cm(8) < stop_dist) {

      drive_goto(0,0);
      hypo = (sqrt(pow(totaldx, 2) + pow(totaldy, 2)))/10;
      thetachange = -atan(totaldx/totaldy) * 57.296;
      print("distance travelled : %.2fcm\n", hypo);
      print("angle : %.2f\n", thetachange);
      pause(1000);
      drive_goto(52,-51);
      atWall = 1;
      current = previous; 
  } else {

    //calculates error value then resets IR sensor values
    errorValue = irRight - irLeft;
    irRight = 0;
    irLeft = 0;

    //resets the total error value when there a large change in setpoint occurs
    if ((prevError > 0 && errorValue <= 0) || (prevError < 0 && errorValue >= 0)) {
      errSum = 0;}

    //further error calculations  
    errSum += errorValue;
    errDiff = errorValue - prevError;
    prevError = errorValue;
    
    corrSpeed = calcSpeedCorrection(errorValue, errSum, errDiff);
    
    int baseSpeed = 50;
    if (errorValue == 0) {
      drive_speed(baseSpeed,baseSpeed);
      current->distLeft = baseSpeed;
      current->distRight = baseSpeed; 
      } else if (errorValue > 0) { 
        drive_speed(baseSpeed+corrSpeed, baseSpeed);
        current->distLeft = baseSpeed+corrSpeed;
        current->distRight = baseSpeed;
        } else if (errorValue < 0) {
          drive_speed(baseSpeed, baseSpeed-corrSpeed);
          current->distLeft = baseSpeed;
          current->distRight = baseSpeed-corrSpeed;
        }

   current->prev = previous;
   previous->next = current;
   pause(50);
   previous = current;
   
   recordDist();
}
  
 } else {
     //backtracking using linked list in reverse order
   if (previous->prev != NULL) {
     drive_speed(previous->distRight, previous->distLeft);
     previous = previous->prev;
     pause(105);
     } else drive_speed(0,0);
   }
  }
  return 0;
}

