#pragma config(Sensor, S1, snsr_accel, sensorI2CCustom)
#include "drivers/hitechnic-accelerometer.h"
//#include <HTAC-driver.h>
#include "datlog.h"

#define LOG_VAL accel_x
#define COUNTDOWN 5
#define MAX_FILESIZE 100
#define FILENAME "test"
#define ARRAY_LEN 1800

int i;
int temp[3][ARRAY_LEN];
int time[ARRAY_LEN];

void readAccel(int &x, int &y, int &z);
void checkFileLength(int &file_length, int &file_number);

task main() {
    int counter, counter_write, counter_file;

    i = 0;
		ClearTimer(T2);
    ClearTimer(T1);
    nxtDisplayCenteredTextLine(2, "%d", time1[T1]);
    while(time1[T1] < (COUNTDOWN * 1000)) {
    		if (time1[T2] >= 4500){
    			motor[motorB] = 0;
    			motor[motorC] = 0;
    		}else if (time1[T2] >= 500){
    			motor[motorB] = 100;
    			motor[motorC] = 100;
    		}
        readAccel(temp[0][i], temp[1][i], temp[2][i]);
        time[i++] = time1[T1];
    }

    motor[motorB] = 0;
    motor[motorC] = 0;

    counter = 0;
    counter_write = 0;
    counter_file = 1;

    string file = FILENAME;
    strcat(file,"1.dat");

    bOpenWriteTextFile(file, kMaxFileSize);

    //for(i = 0; i < ARRAY_LEN; i++)
    i=0;
    while (time[i] > 0)
    	{
        writeIntegerNumber(counter++);
        writeIntegerNumber(time[i]);
        writeIntegerNumber(temp[0][i]);
        writeIntegerNumber(temp[1][i]);
        writeIntegerNumber(temp[2][i]);
        writeNewLine();
        counter_write++;
        checkFileLength(counter_write, counter_file);
        i++;
    }

    closeWriteTextFile();
    StopAllTasks();
}

void readAccel(int &x, int &y, int &z) {
    HTACreadAllAxes(snsr_accel, x, y, z);
    return;
}

void checkFileLength(int &file_length, int &file_number) {
    if(file_length > MAX_FILESIZE) {
        file_length = 0;
        closeWriteTextFile();
        file_number++;
        string str = "";
        string new_file;
        strcat(new_file,FILENAME);
        StringFormat(str,"%d",file_number);
        strcat(new_file,str);
        strcat(new_file,".dat");
        nxtDisplayCenteredTextLine(3, "%s", new_file);
        wait10Msec(100);
        bOpenWriteTextFile(new_file, kMaxFileSize);
    }
    return;
}
