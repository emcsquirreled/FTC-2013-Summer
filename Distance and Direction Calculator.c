#pragma config(Sensor, S1,     GYRO,    sensorAnalogInactive)

#include "drivers/hitechnic-gyro.h"

// Current heading of the robot
float currHeading = 0;
float goalHeading = 0;

string str = "";
// Variables for the PID controller
float proportional = 0;
float integral = 0;
float differential = 0;
float Kp = 0.75;
float Ki = 0;
float Kd = 0;
float error = 0;
float errorSum = 0;
float previousError = 0;
//This has to be an int because the motoros will not accept float values
int correction = 0;
int distanceRight = 5;
int distanceUp = 25;

void driveDistance(int cm, int power);
void alignToHeading();
// Task to keep track of the current heading using the HT Gyro
task getHeading () {
	float delTime = 0;
	float prevHeading = 0;
	float curRate = 0;

  HTGYROstartCal(GYRO);
  PlaySound(soundBeepBeep);
  while (true) {
    time1[T1] = 0;
    curRate = HTGYROreadRot(GYRO);
    if (abs(curRate) > 5 || abs(curRate) < -5) {
      prevHeading = currHeading;
      currHeading = prevHeading + curRate * delTime;
      if (currHeading > 360) currHeading -= 360;
      else if (currHeading < 0) currHeading += 360;
    }
    wait1Msec(5);
    delTime = ((float)time1[T1]) / 1000;
    //delTime /= 1000;
  }
}
void alignToHeading() {
	while (abs(currHeading-goalHeading)>1)	{
			previousError = error;
			error = currHeading - goalHeading;
			errorSum = errorSum + error;
			proportional = error * Kp;
			integral = errorSum * Ki;
			differential = (previousError - error) * Kd;
			correction = differential + integral + proportional;
			motor[motorB] = correction;
			motor[motorC] = -correction;
			wait1Msec(5);
	}

}

task main() {
	goalHeading = radiansToDegrees(atan(distanceRight/distanceUp));
	StartTask(getHeading);
	alignToHeading();
	driveDistance(sqrt(distanceUp * distanceUp + distanceRight * distanceRight), 70);
	while (true){
		StringFormat(str,"%f1.5",currHeading);
		nxtDisplayCenteredBigTextLine(4,str);
	}
}

void driveDistance(int cm, int power)
{
  nMotorEncoder[motorC] = 0; // It is good practice to reset encoder values at the start of a function.

  //Calculate inches by multiplying the ratio we determined earlier with the amount of
  //inches to go, then divide by ten as the ratio used is for an inch value.
  //Since we don't want to calculate every iteration of the loop, we will find the clicks needed
  //before we begin the loop.
  int tickGoal = (360 * cm) / 27;

  while(nMotorEncoder[motorC] < tickGoal)
  {
    motor[motorC] = power;  // The nice thing about encoders is that we can use any power value we want, and
    motor[motorB] = power; // still get the same distance.
  }
  motor[motorC] = 0; // Stop the loop once the encoders have counted up the correct number of encoder ticks.
  motor[motorB] = 0;
}
