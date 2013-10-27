#include <stdio.h>
//#include "stm32f4xx.h"
//#include "stm32f4xx_conf.h"

#include "accelerometer.h"
#include "filter.h"
#include "timer.h"

#define NO_TIMEOUT 0
#define TIMEOUT_OCCURRED 1

short interruptStatus;

int main()
{
	int32_t values[3];
	float tilts[3];

	initAccelerometer();
	
	movingAverageFilter filterX;
	movingAverageFilter filterY;
	movingAverageFilter filterZ;
	
	initializeFilter(&filterX);
	initializeFilter(&filterY);
	initializeFilter(&filterZ);
	
	initTimer();

	
	while(1) {
		
		// Wait for a timer 3 event to occur
		while(interruptStatus != TIMEOUT_OCCURRED);
		interruptStatus = NO_TIMEOUT;
		
		getAcceleration(values);
		
		updateFilter(&filterX, values[0]);
		updateFilter(&filterY, values[1]);
		updateFilter(&filterZ, values[2]);
		
		getTilt(values, tilts);
		
		printf("X acc: %d\n", values[0]);
		printf("Y acc: %d\n", values[1]);
		printf("Z acc: %d\n", values[2]);
		printf("X tilt: %f\n", tilts[0]);
		printf("Y tilt: %f\n", tilts[1]);
		printf("Z tilt: %f\n", tilts[2]);
		printf("Pitch: %f\n", (180 / PI)	* getPitch(values));
		printf("Roll: %f\n\n", (180 / PI)	* getRoll(values));
	}
	
}


