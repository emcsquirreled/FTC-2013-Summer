#pragma config(Sensor, S3,     GYRO,    sensorAnalogInactive)
#include "Direction.h"
task main()
{
	StartTask(getHeading);
	time1[T2] = 0;
	while (true){
		nxtDisplayCenteredBigTextLine(4,"%f",currHeading);
		if (time1[T2] > 2000 && time1[T2] < 5000){
			setGatheringData(false);
		}else if (time1[T2] >= 5000){
			setGatheringData(true);
		}
	}

}
