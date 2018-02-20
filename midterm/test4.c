#include "simpletools.h"
#include "abdrive.h"
#include "ping.h"
#include <stdlib.h>
#include "mylib.h"


int main() {

  driveForward(10);

  double displacement = sqrt(pow(dX, 2) + pow(dY, 2));
  double angle = atan(dY / dX);
  printf("Displacement: %.2f mm, angle: %.2f rad\n", displacement, angle);
  pause(2000);
  drive_goto(52, -51);
  goBack(82);
  return 0;
}
