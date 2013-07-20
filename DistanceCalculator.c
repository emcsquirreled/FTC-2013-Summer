#pragma config(Sensor, S1, snsr_accel, sensorI2CCustom)
#include "drivers/hitechnic-accelerometer.h"
#include "datlog.h"

#define LOG_VAL accel_x
#define COUNTDOWN 5
#define MAX_FILESIZE 100
#define FILENAME "test"
#define ARRAY_LEN 1800

int i;
int temp[3][ARRAY_LEN];
int time[ARRAY_LEN];

int X = 0;
int Y = 0;
int Z = 0;
int offset = 0;

void checkFileLength(int &file_length, int &file_number);

task main()
{
	float T = 0;
	float A = 0;
	float D = 0;
	float V = 0;
	int counter, counter_write, counter_file;
	ClearTimer(T2);
	ClearTimer(T1);
	HTACreadAllAxes(snsr_accel, offset, Y, Z);
	while (time1[T2] < (COUNTDOWN * 1000)){
		HTACreadAllAxes(snsr_accel, X, Y, Z);
		X = X - offset;
		if (X<5 && X>-5){
			X = 0;
		}
		T = time1[T1] ;// 1000.0;
		ClearTimer(T1);
		// Unit is cm/s^2
		A = X * 4.584;
		D = D + (0.5 * A * T * T)/* + (V * T)*/;
		V = V + (A * T);
		nxtDisplayBigTextLine(3, "%d", D);
		temp[0][i] = A;
		temp[1][i] = V;
		temp[2][i] = D;
    time[i++] = T * 1000;
	}
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
