#include "simpletools.h"
#include "abdrive.h"
#include "ping.h"
#include <stdlib.h>


#define FORWARD_SPEED 30
#define GOBACK_SPEED 82
#define KP_BACKWARD 2
#define Bai 0

#define INC_DELAY_FACTOR 5

#define ROBOT_WIDTH 105.8
#define TICKS_TO_MM 3.249

typedef struct node {
  int leftTicks;
  int rightTicks;
  struct node *next;
} TicksDiffLinked;


TicksDiffLinked *head = NULL;
int speedCorrection;
int previousLeftTicks = 0; 
int previousRightTicks = 0;

double spotDistance[2]={0,0};
double prevDistance[2]={0,0};
double dX = 0;
double dY = 0;
double spotAngle = 0;
double distanceWheels[2]={0,0};
double coord[3]={0,0,0};


void getDistanceWheels()
{
  int ticksList[2];
  drive_getTicks(&ticksList[0], &ticksList[1]);
  distanceWheels[0] = (double) (ticksList[0]) * TICKS_TO_MM;
  distanceWheels[1] = (double) (ticksList[1]) * TICKS_TO_MM;
}

double getAngleChange()
{
  return (distanceWheels[0] - distanceWheels[1]) / ROBOT_WIDTH;
}

double radiusMiddle(double spotAngle)
{
  if(spotAngle != 0)
  {
    double radiusLeft = spotDistance[0] / spotAngle;       
    double radiusRight = spotDistance[1] / spotAngle;
    return (radiusLeft + radiusRight) / 2;
  }
}

void getCoordinates()
{
  getDistanceWheels();
  double deltaAngle = getAngleChange();
  double r = radiusMiddle(spotAngle);

  if (deltaAngle == 0 || spotAngle == 0) {
    coord[0] = spotDistance[0];
    coord[1] = 0;
  }
  else {
    coord[0] = r * sin(spotAngle + deltaAngle) - r * sin(spotAngle);
    coord[1] = r * cos(spotAngle + deltaAngle) - r * cos(spotAngle);
  }
  coord[2] = deltaAngle;
  spotAngle = deltaAngle;
}

void calculateCoordinates()
{
  getDistanceWheels();
  spotDistance[0] = distanceWheels[0] - prevDistance[0];
  spotDistance[1] = distanceWheels[1] - prevDistance[1];
  prevDistance[0] = distanceWheels[0];
  prevDistance[1] = distanceWheels[1];

  getCoordinates(); 
  dX += coord[0];
  dY += coord[1];
  spotAngle = coord[2];
}

int infraredDelta() {
  int left = 0;
  int right = 0;


  for (int dacVal = 0; dacVal < 160; dacVal += 8) {
    dac_ctr(26, 0, dacVal);
    freqout(11, 1, 38000);
    left += input(10);

    dac_ctr(27, 1, dacVal);
    freqout(1, 1, 38000);
    right += input(2);
  }
  return right - left;
}


void tickDeltas(int *leftTicks, int *rightTicks) {
  int currentLeftTicks;
  int currentRightTicks;
  drive_getTicks(&currentLeftTicks, &currentRightTicks);

  *leftTicks = currentLeftTicks - previousLeftTicks;
  *rightTicks = currentRightTicks - previousRightTicks;

  previousLeftTicks = currentLeftTicks;
  previousRightTicks = currentRightTicks;
}

void pushTicks(int rightTicks, int leftTicks) {
  TicksDiffLinked *new = malloc(sizeof(TicksDiffLinked));
  (*new).leftTicks = leftTicks;
  (*new).rightTicks = rightTicks;
  (*new).next = head;
  head = new;
}

void driveForward() {


  for (int delay_increment = 0; ping_cm(8) > 12; delay_increment++){
  
    speedCorrection = infraredDelta() ;
  
    drive_speed(FORWARD_SPEED + speedCorrection, FORWARD_SPEED - speedCorrection);
  
  
    if (delay_increment % INC_DELAY_FACTOR == 0) {
      int currentLeft;
      int currentRight;
      tickDeltas(&currentLeft, &currentRight);
      pushTicks(currentRight, currentLeft);
      calculateCoordinates();
    }
  }


  drive_speed(0, 0);
}

void updateTickDeltas() {
  int currentLeftTicks;
  int currentRightTicks;
  drive_getTicks(&currentLeftTicks, &currentRightTicks);
  previousLeftTicks = currentLeftTicks;
  previousRightTicks = currentRightTicks;
}

void returnBack() {

  updateTickDeltas();


  int leftCorrection;
  int rightCorrection;

  int currentLeftTicks;
  int currentRightTicks;

  int leftTicksRemaining;
  int rightTicksRemaining;

  TicksDiffLinked *iterator = head;
  while (iterator) {
  
    currentLeftTicks = 0;
    currentRightTicks = 0;
    leftTicksRemaining = (*iterator).leftTicks;
    rightTicksRemaining = (*iterator).rightTicks;
  
    while (currentLeftTicks <= (leftTicksRemaining+Bai) 
            && currentRightTicks <= (rightTicksRemaining+Bai)) {
    
      leftCorrection = KP_BACKWARD * (currentLeftTicks - (leftTicksRemaining +Bai));
      rightCorrection = KP_BACKWARD * (currentRightTicks - (rightTicksRemaining +Bai));
    
      drive_speed(GOBACK_SPEED + leftCorrection, GOBACK_SPEED + rightCorrection);
    
      drive_getTicks(&currentLeftTicks, &currentRightTicks);
      currentLeftTicks -= previousLeftTicks;
      currentRightTicks -= previousRightTicks;
    }
  
  
  
    drive_getTicks(&previousLeftTicks, &previousRightTicks);
    iterator = (*iterator).next;
  }


  drive_speed(0, 0);
}