#include "simpletools.h"
#include "abdrive.h"
#include "ping.h"
#include <stdlib.h>

typedef struct nodes {
  int leftTicks;
  int rightTicks;
  struct nodes *nextNode;
} linkedList;

linkedList *head = NULL;
int preLTicks = 0;
int preRTicks = 0;
int speedCorrection;
float dX = 0;
float dY = 0;
float distanceWheelsl = 0;
float distanceWheelsr = 0;
float coordiante[2]={0,0};
float coordAngle = 0;
float currentDistance[2]={0,0};
float prevDistance[2]={0,0};
float currentAngle = 0;

void distanceDiff()
{
  int templist[2];
  drive_getTicks(&templist[0], &templist[1]);
  distanceWheelsl = templist[0] * 3.249;
  distanceWheelsr = templist[1] * 3.249;
}

float radiusFromMiddle(float currentAngle)
{
  if(currentAngle != 0)
  {
    float rLeft = currentDistance[0] / currentAngle;       
    float rRight = currentDistance[1] / currentAngle;
    return (rLeft + rRight) / 2;
  }
  else
  {
	return 999.0;
  }
}

void tickDiff(int *leftTicks, int *rightTicks) {
  int currentLeftTicks, currentRightTicks;
  drive_getTicks(&currentLeftTicks, &currentRightTicks);
  *leftTicks = currentLeftTicks - preLTicks;
  *rightTicks = currentRightTicks - preRTicks;
  preLTicks = currentLeftTicks;
  preRTicks = currentRightTicks;
}

void addTicksList(int rightTicks, int leftTicks) {
  linkedList *new = malloc(sizeof(linkedList)*2);
  (*new).leftTicks = leftTicks;
  (*new).rightTicks = rightTicks;
  (*new).nextNode = head;
  head = new;
}

void getCoordinates()
{
  float deltaAngle;
  float r;
  deltaAngle = (distanceWheelsl - distanceWheelsr) / 105.8;
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
  currentDistance[1] = distanceWheelsr - prevDistance[1];
  currentDistance[0] = distanceWheelsl - prevDistance[0];
  prevDistance[1] = distanceWheelsr;
  prevDistance[0] = distanceWheelsl;
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

void driveForward(int speed) {
  int interval = 0;
  while (ping_cm(8) > 11)
  {
    interval += 1;
    speedCorrection = infraredDiff() * 0.75;
    drive_speed(speed + speedCorrection, speed - speedCorrection);
    if (interval == 30) 
    {
      int currentLeft;
      int currentRight;
      tickDiff(&currentLeft, &currentRight);
      addTicksList(currentRight, currentLeft);
      calculateCoordinates();
      interval = 0;
    }
  }
  drive_speed(0, 0);
}
float getdX()
{
  return dX;
}
float getdY()
{
  return dY;
}

void goBack(int speed) {
  int currentLeftTicks,currentRightTicks;
  int leftCorrection,rightCorrection;
  int leftTicksLeft, rightTicksLeft;

  drive_getTicks(&currentLeftTicks, &currentRightTicks);
  preLTicks = currentLeftTicks;
  preRTicks = currentRightTicks;
  linkedList *iteration = head;
  while (iteration!= NULL) 
  {
    currentRightTicks = 0;
    currentLeftTicks = 0;
    rightTicksLeft = (*iteration).rightTicks;
    leftTicksLeft = (*iteration).leftTicks;
    while (currentLeftTicks < (leftTicksLeft + 2)&& currentRightTicks < (rightTicksLeft + 2)) 
    {
      drive_speed(speed + 3.7 * (currentLeftTicks - (leftTicksLeft + 1)), speed + 3.7 * (currentRightTicks - (rightTicksLeft + 1)));
      drive_getTicks(&currentLeftTicks, &currentRightTicks);
      currentLeftTicks = currentLeftTicks - preLTicks;
      currentRightTicks = currentRightTicks - preRTicks;
    }
    drive_getTicks(&preLTicks, &preRTicks);
    iteration = (*iteration).nextNode;
  }
  drive_speed(0, 0);
}
