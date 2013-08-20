#include "drivers/hitechnic-gyro.h"

// Testing variable
float curRate = 0;

// Does stuff
int time = 0;
// Stores the most recent heading value
float currHeading = 0;

// This stores true if the function is gathering data, and false if it is not
bool gatheringData = true;

// Use to change whether it is gathering gyro readings or not
void setGatheringData(bool gathering);

// Stores the current heading relative to the previous heading
// in currHeading
task getHeading () {
	float delTime = 0;
	float prevHeading = 0;
	float curRate = 0;

  HTGYROstartCal(GYRO);
  while (true) {
    time1[T1] = 0;
    curRate = HTGYROreadRot(GYRO);
    if (gatheringData == true){
    	if (abs(curRate) > 5 || abs(curRate) < -5) {
      	prevHeading = currHeading;
      	currHeading = prevHeading + curRate * delTime;
      	if (currHeading > 360) currHeading -= 360;
      	else if (currHeading < 0) currHeading += 360;
    	}
  	}
    wait1Msec(5);
    delTime = ((float)time1[T1]) / 1000;
    //delTime /= 1000;
  }
}

void setGatheringData(bool gathering){
	gatheringData = gathering;
}
