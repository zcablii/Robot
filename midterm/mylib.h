#include "simpletools.h"
#include "abdrive.h"
#include "ping.h"
#include <stdlib.h>


typedef struct each {
  int leftTicks;
  int rightTicks;
  struct each *next;
} linkedList;

linkedList *head = NULL;

int preLTicks = 0;
int preRTicks = 0;
int speedCorrection;
double dX = 0;
double dY = 0;
double distanceWheels[2]={0,0};
double coordiante[2]={0,0};
double coordAngle = 0;
double currentDistance[2]={0,0};
double prevDistance[2]={0,0};
double currentAngle = 0;

void distanceDiff()
{
  int templist[2];
  drive_getTicks(&templist[0], &templist[1]);
  distanceWheels[0] = (double) (templist[0]) * 3.249;
  distanceWheels[1] = (double) (templist[1]) * 3.249;
}

double radiusFromMiddle(double currentAngle)
{
  if(currentAngle != 0)
  {
    double rLeft = currentDistance[0] / currentAngle;       
    double rRight = currentDistance[1] / currentAngle;
    return (rLeft + rRight) / 2;
  }
}

void pushTicks(int rightTicks, int leftTicks) {
  linkedList *new = malloc(sizeof(linkedList));
  (*new).leftTicks = leftTicks;
  (*new).rightTicks = rightTicks;
  (*new).next = head;
  head = new;
}

void getCoordinates()
{
  double deltaAngle;
  double r;
  deltaAngle = (distanceWheels[0] - distanceWheels[1]) / 105.8;
  r = radiusFromMiddle(currentAngle);
  if (deltaAngle == 0 || currentAngle == 0) {
    coordiante[0] = currentDistance[0];
    coordiante[1] = 0;
  }

  else 
  {
    coordiante[0] = r * sin(currentAngle + deltaAngle) - r * sin(currentAngle);
    coordiante[1] = r * cos(currentAngle + deltaAngle) - r * cos(currentAngle);
  }
  coordAngle = deltaAngle;
  currentAngle = deltaAngle;
}

void calculateCoordinates()
{
  distanceDiff();
  currentDistance[1] = distanceWheels[1] - prevDistance[1];
  currentDistance[0] = distanceWheels[0] - prevDistance[0];
  prevDistance[1] = distanceWheels[1];
  prevDistance[0] = distanceWheels[0];
  getCoordinates(); 
  dY += coordiante[1];
  dX += coordiante[0];
  currentAngle = coordAngle;
}

int infraredDiff() {
  int leftdis = 0;
  int rightdis = 0;
  for (int dacVal = 0; dacVal < 160; dacVal += 8) 
  {
    dac_ctr(26, 0, dacVal);
    freqout(11, 1, 38000);
    leftdis = leftdis + input(10);
    dac_ctr(27, 1, dacVal);
    freqout(1, 1, 38000);
    rightdis = rightdis + input(2);
  }
  return rightdis - leftdis;
}


void tickDiff(int *leftTicks, int *rightTicks) {
  int currentLeftTicks, currentRightTicks;
  drive_getTicks(&currentLeftTicks, &currentRightTicks);
  *leftTicks = currentLeftTicks - preLTicks;
  *rightTicks = currentRightTicks - preRTicks;
  preLTicks = currentLeftTicks;
  preRTicks = currentRightTicks;
}


void driveForward(int speed) {

  for (int delay_increment = 0; ping_cm(8) > 11; delay_increment++){
  
    speedCorrection = infraredDiff() * 0.75;
    drive_speed(speed + speedCorrection, speed - speedCorrection);
    if (delay_increment % 30 == 0) {
      int currentLeft;
      int currentRight;
      tickDiff(&currentLeft, &currentRight);
      pushTicks(currentRight, currentLeft);
      calculateCoordinates();
    }
  }
  drive_speed(0, 0);
}


void goBack(int speed) {
  int curLeftTicks;
  int curRightTicks;
  drive_getTicks(&curLeftTicks, &curRightTicks);
  preLTicks = curLeftTicks;
  preRTicks = curRightTicks;

  int currentLeftTicks, currentRightTicks;
  int leftCorrection,rightCorrection;
  int leftTicksLeft, rightTicksLeft;
  linkedList *iterator = head;
  while (iterator!= NULL) 
  {
    currentRightTicks = 0;
    currentLeftTicks = 0;
    rightTicksLeft = (*iterator).rightTicks;
    leftTicksLeft = (*iterator).leftTicks;
    while (currentLeftTicks < (leftTicksLeft + 2) 
            && currentRightTicks < (rightTicksLeft + 2)) {
    
      drive_speed(speed + 3.7 * (currentLeftTicks - (leftTicksLeft + 1)), speed + 3.7 * (currentRightTicks - (rightTicksLeft + 1)));
      drive_getTicks(&currentLeftTicks, &currentRightTicks);
      currentLeftTicks = currentLeftTicks - preLTicks;
      currentRightTicks = currentRightTicks - preRTicks;
    }
    drive_getTicks(&preLTicks, &preRTicks);
    iterator = (*iterator).next;
  }
  drive_speed(0, 0);
}