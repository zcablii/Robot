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
double dX = 0;
double dY = 0;
double distanceWheelsl = 0;
double distanceWheelsr = 0;
double coordiante[2]={0,0};
double coordAngle = 0;
double currentDistance[2]={0,0};
double prevDistance[2]={0,0};
double currentAngle = 0;

void distanceDiff()
{
  int templist[2];
  drive_getTicks(&templist[0], &templist[1]);
  distanceWheelsl = templist[0] * 3.249;
  distanceWheelsr = templist[1] * 3.249;
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

void tickDiff(int *leftTicks, int *rightTicks) {
  int currentLeftTicks, currentRightTicks;
  drive_getTicks(&currentLeftTicks, &currentRightTicks);
  *leftTicks = currentLeftTicks - preLTicks;
  *rightTicks = currentRightTicks - preRTicks;
  preLTicks = currentLeftTicks;
  preRTicks = currentRightTicks;
}

void addTicksList(int rightTicks, int leftTicks) {
  linkedList *new = malloc(sizeof(linkedList));
  (*new).leftTicks = leftTicks;
  (*new).rightTicks = rightTicks;
  (*new).nextNode = head;
  head = new;
}

void getCoordinates()
{
  double deltaAngle;
  double r;
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

  for (int interval = 0; ping_cm(8) > 11; interval++){
    speedCorrection = infraredDiff() * 0.75;
    drive_speed(speed + speedCorrection, speed - speedCorrection);
    if (interval % 30 == 0) 
    {
      int currentLeft;
      int currentRight;
      tickDiff(&currentLeft, &currentRight);
      addTicksList(currentRight, currentLeft);
      calculateCoordinates();
    }
  }
  drive_speed(0, 0);
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